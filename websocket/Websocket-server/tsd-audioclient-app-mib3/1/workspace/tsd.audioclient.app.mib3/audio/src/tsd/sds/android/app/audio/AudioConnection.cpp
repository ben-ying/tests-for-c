//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioConnection.cpp
/// @brief Implementation of class AudioConnection
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/AudioConnection.hpp"

#include <tsd/common/system/MutexGuard.hpp>
#include <tsd/sds/android/common/NullCheck.hpp>
#include <algorithm>
#include "tsd/sds/android/app/audio/rsiapi/IAudioConnectionsListener.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

DEF_LOGGER(AudioConnection);

AudioConnection::AudioConnection(tsd::common::types::SmartPtr<IAudioSource> audioChannel,
                                 const std::string &connectionId,
                                 const std::string& channelId)
   : m_AudioChannel(audioChannel)
   , m_Listeners()
   , m_ConnectionId(connectionId)
   , m_ChannelId(channelId)
   , m_Mutex()
{
   ANDROID_THROW_IF_NULL(m_AudioChannel, AudioConnection);
}


AudioConnection::~AudioConnection()
{
}

bool AudioConnection::registerListener(IAudioConnectionListener *listener)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   bool result = listener != nullptr;
   m_Log.logInfo(__func__) << std::hex << listener;
   if(result)
   {
      m_Listeners.insert(listener);
   }
   return result;
}

void AudioConnection::cancelListenerRegistration(IAudioConnectionListener *listener)
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << std::hex << listener;
   m_Listeners.erase(listener);
}

const std::string &AudioConnection::getId()
{
   return m_ConnectionId;
}

void AudioConnection::start()
{
   m_Log.logInfo(__func__) << m_ConnectionId;
   m_AudioChannel->start(m_ConnectionId);
}

void AudioConnection::stop()
{
   m_Log.logInfo(__func__) << m_ConnectionId;
   m_AudioChannel->stop(m_ConnectionId);
}

void AudioConnection::initialize()
{
   m_AudioChannel->registerListener(m_ConnectionId, this);
   m_Log.logInfo(__func__) << "Initialized AudioConnection with id: " << m_ConnectionId;
}

void AudioConnection::started()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   listenerOperation([](IAudioConnectionListener* listener){listener->audioConnectionStarted();});
}

void AudioConnection::onAudioConnectionMuted()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   if(m_Listeners.empty())
   {
      // if no listener is registered anymore, we asume, that no playback is running anymore.
      m_AudioChannel->stop(m_ConnectionId);
   }
   else
   {
      listenerOperation([](IAudioConnectionListener* listener){listener->onAudioConnectionMuted();});
   }
}

void AudioConnection::stopped()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   listenerOperation([](IAudioConnectionListener* listener){listener->audioConnectionStopped();});
}

void AudioConnection::paused()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   listenerOperation([](IAudioConnectionListener* listener){listener->audioConnectionPaused();});
}

void AudioConnection::startFailed()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   listenerOperation([](IAudioConnectionListener* listener){listener->audioConnectionStartFailed();});
}

void AudioConnection::stopFailed()
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   m_Log.logInfo(__func__) << m_ConnectionId;
   listenerOperation([](IAudioConnectionListener* listener){listener->audioConnectionStopFailed();});
}

void AudioConnection::listenerOperation(std::function<void (IAudioConnectionListener *)> func)
{
   for(auto element : m_Listeners)
   {
      if(nullptr != element)
      {
         func(element);
      }
      else
      {
         m_Log.logWarn(__func__) << "Found null in listener list";
      }
   }
}

} // audio
} // app
} // android
} // sds
} // tsd

