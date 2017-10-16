//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioSource.cpp
/// @brief Implementation of class AudioSource
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/AudioSource.hpp"

#include <tsd/sds/android/common/NullCheck.hpp>
#include <functional>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

static const std::string& toString(const rsiapi::AudioStatus status)
{
   static const std::string fadedIn("fadedIn");
   static const std::string muted("muted");
   static const std::string readyToFade("readyToFade");
   static const std::string undefined("undefined");
   static const std::string unknown("unknown");

   switch(status)
   {
   case rsiapi::AudioStatus_fadedIn: return fadedIn;
   case rsiapi::AudioStatus_muted: return muted;
   case rsiapi::AudioStatus_readyToFade: return readyToFade;
   case rsiapi::AudioStatus_undefined: return undefined;
   }
   return unknown;
}

static const std::string& toString(const rsiapi::PlayStatus status)
{
   static const std::string idle("idle");
   static const std::string playing("playing");
   static const std::string undefined("undefined");
   static const std::string unknown("unknown");

   switch(status)
   {
   case rsiapi::PlayStatus_idle: return idle;
   case rsiapi::PlayStatus_playing: return playing;
   case rsiapi::PlayStatus_undefined: return undefined;
   }
   return unknown;
}

static const std::string& toString(const rsiapi::ConnectionStatus status)
{
   static const std::string paused("paused");
   static const std::string stopped("stopped");
   static const std::string started("started");
   static const std::string unknown("unknown");

   switch(status)
   {
   case rsiapi::ConnectionStatus_paused: return paused;
   case rsiapi::ConnectionStatus_started: return started;
   case rsiapi::ConnectionStatus_stopped: return stopped;
   }
   return unknown;
}


DEF_LOGGER(audio::AudioSource);

AudioSource::AudioSource(tsd::common::types::SmartPtr<tsd::sds::utils::threads::SimpleWorkerThread> workerThread,
                         IAudioSourceListener *audioSourceListener,
                         const std::string &channelId,
                         bool useAudioManagement)
   : m_WorkerThread(workerThread)
   , m_AudioSourceListener(audioSourceListener)
   , m_Source(channelId)
   , m_Listeners()
   , m_SourceControl(nullptr)
   , m_UseAudioManagement(useAudioManagement)
   , m_RequestedConnection()
   , m_ActiveConnection()
   , m_AudioStatus(rsiapi::AudioStatus_undefined)
   , m_PlayStatus(rsiapi::PlayStatus_undefined)
   , m_ConnectionStates()
   , m_Mutex()
{
   ANDROID_THROW_IF_NULL(m_WorkerThread, AudioSource);
}


AudioSource::~AudioSource()
{
}


tsd::common::types::SmartPtr<rsiapi::ISourceControl> AudioSource::initialize()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   bool result = false;
   if (m_UseAudioManagement)
   {
      m_SourceControl = rsiapi::createSourceControl(m_Source, this, this).release();
      if(nullptr == m_SourceControl.get())
      {
         m_Log.logError(__func__) << "Failed to start connection";
      }
      else if(m_SourceControl->connect())
      {
         m_Log.logInfo(__func__) << "connected to source control.";
         std::vector<std::string> connections = m_SourceControl->getCurrentAudioConnections();
         m_Log.logInfo(__func__) << "Have connections: " << connections.size();
         for(auto con : connections)
         {
            m_Log.logInfo(__func__) << con;
         }
         result = true;


      }
      else
      {
         m_Log.logError(__func__) << "Could not connect to source control.";
      }
   }
   else
   {
      result = true;
   }
   m_Log.logInfo(__func__) << "Initialized audio channel " << m_Source << " with sourcecontrol: " << (result ? "successful" : "not successful");

   return m_SourceControl;
}


bool AudioSource::registerListener(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   bool success = false;
   m_Log.logTrace(__func__) << "Listener for connection '" << connectionName << "' registered: " << std::hex << listener;
   if (!connectionName.empty() && nullptr != listener)
   {
      m_Listeners.emplace_front(ListenerDescription{connectionName, listener});
      success = true;
   }
   return success;
}


void AudioSource::cancelListenerRegistration(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   AudioSource::ListenerDescription description(connectionName, listener);
   m_Listeners.remove(description);
}


void AudioSource::start(const std::string& connectionName)
{
   if(m_UseAudioManagement)
   {
      m_WorkerThread->push([this, connectionName]{processStart(connectionName);});
   }
   else
   {
      m_WorkerThread->push([this,connectionName]{ doCallback(connectionName, [](ListenerDescription& desc){desc.second->started();});});
   }
}


void AudioSource::stop(const std::string& connectionName)
{
   if(m_UseAudioManagement)
   {
      m_WorkerThread->push([this, connectionName]{processStop(connectionName);});
   }
   else
   {
      m_WorkerThread->push([this,connectionName]{ doCallback(connectionName, [](ListenerDescription& desc){desc.second->stopped();});});
   }

}


void AudioSource::onSourceAvailabilityChanged(bool available)
{
   m_WorkerThread->push([this, available]{processSourceAvailabilityChange(available);});
}


void AudioSource::onAudioStatusChanged(const rsiapi::AudioStatus newStatus)
{
   m_WorkerThread->push([this, newStatus]{processAudioStateChange(newStatus);});
}


void AudioSource::onPlayStatusChanged(const rsiapi::PlayStatus newStatus)
{
   m_WorkerThread->push([this, newStatus]{processPlayStateChange(newStatus);});
}


void AudioSource::onAudioConnectionListChanged(const std::vector<std::string> &/*newList*/)
{
   m_Log.logTrace(__func__);
}


void AudioSource::onAudioConnectionStatusChanged(const std::string &connectionName, const rsiapi::ConnectionStatus newStatus)
{
   m_WorkerThread->push([this, connectionName, newStatus]{processConenctionStateChange(connectionName, newStatus);});
}

void AudioSource::processStart(const std::string &connectionName)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "Trying to change connection status to 'started' for connection " << connectionName << " in source " << m_Source;
   if(!m_RequestedConnection.empty())
   {
      m_Log.logInfo(__func__) << "Another connection request is pending. Queueing connection request for '" << connectionName;
      m_RequestQueue.push(connectionName);
   }
   else if(m_SourceControl->changeConnectionStatus(connectionName, rsiapi::ConnectionStatus_started))
   {
      m_RequestedConnection = connectionName;
   }
   else
   {
      m_Log.logWarn(__func__) << "Could not change connection status to 'started' for connection " << connectionName << " in source " << m_Source;
      doCallback(connectionName, [](ListenerDescription& desc){ desc.second->startFailed();});
   }
}

void AudioSource::processStop(const std::string &connectionName)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "Source: " << m_Source << " connection: " << connectionName;
   if(connectionName == m_ActiveConnection)
   {
      if(rsiapi::AudioStatus_muted == m_AudioStatus)
      {
         doCallback(connectionName,[](ListenerDescription& desc){ desc.second->stopped();});
         m_ActiveConnection = "";
      }
      else
      {
         m_Log.logInfo(__func__) << "Trying to change connection status to 'stopped' for connection " << connectionName << " in source " << m_Source;
         if(!m_SourceControl->changeConnectionStatus(connectionName, rsiapi::ConnectionStatus_stopped))
         {
            m_Log.logError(__func__) << "Could not change connection status to 'stopped' for connection " << connectionName << " in source " << m_Source;
         }
      }
   }
   else
   {
      doCallback(connectionName,[](ListenerDescription& desc){ desc.second->stopped();});
   }
}

void AudioSource::processAudioStateChange(const rsiapi::AudioStatus newStatus)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "Audio status changed to '" << toString(newStatus) << "' for source " << m_Source;
   m_Log.logInfo(__func__) << "Active Connection:      " << m_ActiveConnection;
   const rsiapi::AudioStatus oldStatus = m_AudioStatus;
   m_AudioStatus = newStatus;
   if(rsiapi::AudioStatus_muted == m_AudioStatus)
   {
      if(!m_ActiveConnection.empty())
      {
         m_Log.logInfo(__func__) << "Calling onAudioConnectionMuted for connection '" << m_ActiveConnection << "'";
         doCallback(m_ActiveConnection, [](ListenerDescription& desc){ desc.second->onAudioConnectionMuted();});
         m_ActiveConnection = "";
      }
   }
   else if(rsiapi::AudioStatus_readyToFade == m_AudioStatus)
   {
      if(m_ActiveConnection.empty() && !m_RequestedConnection.empty() && oldStatus == rsiapi::AudioStatus_muted)
      {
         m_ActiveConnection = m_RequestedConnection;
         m_RequestedConnection = "";
         doCallback(m_ActiveConnection, [](ListenerDescription& desc){ desc.second->started();});
         if(!m_RequestQueue.empty())
         {
            start(m_RequestQueue.front());
            m_RequestQueue.pop();
         }
      }
   }
}

void AudioSource::processPlayStateChange(const rsiapi::PlayStatus newStatus)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "Play status changed to '" << toString(newStatus) << "' for source " << m_Source;
   m_PlayStatus = newStatus;
}

void AudioSource::processSourceAvailabilityChange(const bool available)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "Source availability changed to '" << (available ? "available" : "unavailable") << "' for source " << m_Source;
   m_AudioSourceListener->onSourceAvailabilityChanged(m_Source, available);
}

void AudioSource::processConenctionStateChange(const std::string &connectionName, const rsiapi::ConnectionStatus newStatus)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << "New status received for connection '" << connectionName << "': " << toString(newStatus);
   m_ConnectionStates[connectionName] = newStatus;
   if(rsiapi::ConnectionStatus_stopped == newStatus && m_ActiveConnection.empty() && connectionName == m_RequestedConnection)
   {
      m_Log.logInfo(__func__) << "Sending 'startFailed' to listeners for connection '" << m_RequestedConnection << "'";
      doCallback(m_RequestedConnection, [](ListenerDescription& desc){ desc.second->startFailed();});
      m_RequestedConnection = "";
      if(!m_RequestQueue.empty())
      {
         start(m_RequestQueue.front());
         m_RequestQueue.pop();
      }
   }
}

void AudioSource::doCallback(const std::string& connectionName, std::function<void(ListenerDescription&)> callback)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__);
   for(ListenerDescription& element : m_Listeners)
   {
      if(element.first == connectionName)
      {
         m_Log.logDebug(__func__) << "calling listener " << element.first;
         callback(element);
      }
   }
}

} // audio
} // app
} // android
} // sds
} // tsd

