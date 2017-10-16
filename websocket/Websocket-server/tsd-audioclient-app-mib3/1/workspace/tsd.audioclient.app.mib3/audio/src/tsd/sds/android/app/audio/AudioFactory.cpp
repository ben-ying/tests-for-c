//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioFactory.cpp
/// @brief Implementation of class AudioFactory
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/AudioFactory.hpp"

#include <tsd/sds/android/common/NullCheck.hpp>
#include <algorithm>
#include "tsd/sds/android/app/audio/AudioConnection.hpp"
#include "tsd/sds/android/app/audio/AudioSource.hpp"
#include "tsd/sds/android/app/audio/SyncAudioOut.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

DEF_LOGGER(AudioFactory);

AudioFactory::AudioFactory(tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession> session,
                           tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> workerThread,
                           const std::map<std::string, AudioConnectionDescription> &connectionAudioSourceMap,
                           bool useAudioManagement)
   : IAudioFactory()
   , m_Session(session)
   , m_WorkerThread(workerThread)
   , m_ConnectionToAudioSourceMap(connectionAudioSourceMap)
   , m_ConnectionInstanceMap()
   , m_AudioSourceInstanceMap()
   , m_AudioOutInstanceMap()
   , m_Connections()
   , m_Channels()
   , m_UseAudioManagement(useAudioManagement)
{
   ANDROID_THROW_IF_NULL(m_Session, AudioFactory);
   ANDROID_THROW_IF_NULL(m_WorkerThread, AudioFactory);
   initialize();
}

AudioFactory::~AudioFactory()
{

}

std::set<std::string> &AudioFactory::getConnectionIds()
{
   if(m_Connections.empty())
   {
       //greg need to add back
/*      for(auto& it : m_ConnectionToAudioSourceMap)
      {
         m_Connections.insert(it.first);
      }*/
       //greg temp test
       m_Connections.insert("ANN_AMP_TTS_READOUT");
   }
   return m_Connections;
}

std::set<std::string> &AudioFactory::getAudioSourceIds()
{
   if(m_Channels.empty())
   {
       //greg need to add back
/*      for(auto& it : m_ConnectionToAudioSourceMap)
      {
         AudioConnectionDescription& desc = it.second;
         m_Channels.insert(desc.m_AudioChannelId);
      }*/
       //greg temp test
       m_Channels.insert("TTS");
   }
   return m_Channels;
}

tsd::common::types::SmartPtr<audio::IAudioConnection> AudioFactory::getAudioConnection(const std::string &connectionId)
{

   return get(connectionId, m_ConnectionInstanceMap);
}

tsd::common::types::SmartPtr<core::backend::playback::IAudioOut> AudioFactory::getAudioOut(const std::string &sourceId)
{
   return get(sourceId, m_AudioOutInstanceMap);
}

const std::string &AudioFactory::getAudioSourceIdForAudioConnection(const std::string &audioConnection)
{
   static std::string empty;
   auto it = m_ConnectionToAudioSourceMap.find(audioConnection);
   if(it != m_ConnectionToAudioSourceMap.end())
   {
      return it->second.m_AudioChannelId;
   }
   return empty;
}

void AudioFactory::onSourceAvailabilityChanged(const std::string &channelName, const bool available)
{
   m_Log.logInfo(__func__) << "Availability" << (available ? "yes" : "no");
   auto it = m_AudioOutInstanceMap.find(channelName);
   if(it != m_AudioOutInstanceMap.end() && available == true)
   {
      m_Log.logInfo(__func__) << "ChannelName: " << channelName;
      it->second->initialize();
   }
}


void AudioFactory::initialize()
{
   const std::set<std::string>& channels = getAudioSourceIds();
   const std::set<std::string>& connections = getConnectionIds();
   tsd::audio::audiorouter::client::ChannelConfiguration config(tsd::audio::audiorouter::client::SampleFormat::s16le, 1, 22050);

   for(auto channelName : channels)
   {

      tsd::common::types::SmartPtr<core::backend::playback::IAudioOut> audioOutPtr;//Used to pass synthesized audio-samples to the client.
      tsd::common::types::SmartPtr<audio::IAudioSource> channelPtr;//This class provides audio channel management functions.

      audio::SyncAudioOut* syncAudiout(new audio::SyncAudioOut(channelName, m_Session));
      audio::AudioSource* channel(new audio::AudioSource(m_WorkerThread, this, channelName, m_UseAudioManagement));
      syncAudiout->initialize(channel->initialize());

      audioOutPtr = syncAudiout;
      channelPtr = channel;

      m_AudioOutInstanceMap.insert(std::make_pair(channelName, audioOutPtr));
      m_AudioSourceInstanceMap.insert(std::make_pair(channelName, channelPtr ));
   }

   for(auto connectionId : connections)
   {
      auto connectionDescIt = m_ConnectionToAudioSourceMap.find(connectionId);
      if(connectionDescIt == m_ConnectionToAudioSourceMap.end())
      {
         m_Log.logError(__func__) << "Could not find channel for connectionId '" << connectionId << "'";
         continue;
      }
      std::string channelName = connectionDescIt->second.m_AudioChannelId;
      tsd::common::types::SmartPtr<audio::IAudioSource> channelPtr = get(channelName, m_AudioSourceInstanceMap);
      if(nullptr == channelPtr.get())
      {
         m_Log.logError(__func__) << "Channel instance is null for channel '" << channelName << "'";
         continue;
      }

      audio::AudioConnection* connection(new audio::AudioConnection(channelPtr, connectionId, channelName));
      connection->initialize();
      tsd::common::types::SmartPtr<audio::IAudioConnection> connectionPtr(connection);//This class provides audio connection management functions.
      m_ConnectionInstanceMap.insert(std::make_pair(connectionId, connectionPtr));
   }
}

} // audio
} // app
} // android
} // sds
} // tsd

