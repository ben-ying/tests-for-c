//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioFactory.hpp
/// @brief Definition of class AudioFactory
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_AUDIOFACTORY_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_AUDIOFACTORY_HPP

#include <tsd/sds/android/core/backend/playback/IAudioOut.hpp>
#include <tsd/sds/utils/threads/SimpleWorkerThread.hpp>
#include <tsd/common/types/typedef.hpp>
#include <tsd/audio/audiorouter/client/ISession.hpp>
#include <functional>
#include <map>
#include <set>

#include <tsd/sds/android/app/audio/AudioConnectionDescription.hpp>
#include "tsd/sds/android/app/audio/IAudioFactory.hpp"
#include "tsd/sds/android/app/audio/IAudioSourceListener.hpp"
#include "tsd/sds/android/app/audio/rsiapi/IAudioConnectionsListener.hpp"
#include "tsd/sds/android/app/audio/rsiapi/ISourceControl.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {


/**
 * @brief Implementation of IAudioFactory.
 */
class AudioFactory : public IAudioFactory, public IAudioSourceListener
{
public:

   /**
    * @brief AudioFactory
    * @param[in] session needed to create SyncAudioOut instances
    * @param[in] workerThread needed to create AudioSource instances
    * @param[in] connectionAudioSourceMap contains the configuration for this instance (all the audio connections and audio source names).
    * @param[in] useAudioManagement passed to the created AudioSource instances.
    */
   AudioFactory(tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession> session,
                tsd::common::types::SmartPtr<tsd::sds::utils::threads::SimpleWorkerThread> workerThread,
                const std::map<std::string, AudioConnectionDescription>& connectionAudioSourceMap,
                bool useAudioManagement);
   virtual ~AudioFactory();

   /**
    * @brief @see IAudioFactory
    */
   virtual std::set<std::string>& getConnectionIds() override;

   /**
    * @brief @see IAudioFactory
    */
   virtual std::set<std::string>& getAudioSourceIds() override;

   /**
    * @brief @see IAudioFactory
    */
   virtual tsd::common::types::SmartPtr<audio::IAudioConnection> getAudioConnection(const std::string& connectionId) override;

   /**
    * @brief @see IAudioFactory
    */
   virtual tsd::common::types::SmartPtr<core::backend::playback::IAudioOut> getAudioOut(const std::string& sourceId) override;

   /**
    * @brief @see IAudioFactory
    */
   virtual const std::string& getAudioSourceIdForAudioConnection(const std::string& audioConnection) override;


private:

   tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession> m_Session;
   tsd::common::types::SmartPtr<tsd::sds::utils::threads::SimpleWorkerThread> m_WorkerThread;

   std::map<std::string, AudioConnectionDescription> m_ConnectionToAudioSourceMap;
   std::map<std::string, tsd::common::types::SmartPtr<audio::IAudioConnection> > m_ConnectionInstanceMap;
   std::map<std::string, tsd::common::types::SmartPtr<audio::IAudioSource> > m_AudioSourceInstanceMap;
   std::map<std::string, tsd::common::types::SmartPtr< core::backend::playback::IAudioOut> > m_AudioOutInstanceMap;
   std::set<std::string> m_Connections;
   std::set<std::string> m_Channels;
   bool m_UseAudioManagement;


   /**
    * @see IAudioChannelListener
    */
   virtual void started() override {}


   /**
    * @see IAudioChannelListener
    */
   virtual void onAudioConnectionMuted() override {}

   /**
    * @see IAudioChannelListener
    */
   virtual void stopped() override {}

   /**
    * @see IAudioChannelListener
    */
   virtual void paused() override {}

   /**
    * @see IAudioChannelListener
    */
   virtual void startFailed() override {}

   /**
    * @see IAudioChannelListener
    */
   virtual void stopFailed() override {}

   /**
    * @brief IAudioSourceListener
    */
   virtual void onSourceAvailabilityChanged(const std::string& channelName, const bool available) override;

   template<typename T>
   T get(const std::string& key, std::map<std::string, T >& map)
   {
      T result(nullptr);
      auto it = map.find(key);
      if(it != map.end())
      {
         result = it->second;
      }
      return result;
   }

   virtual void initialize();

   DECL_LOGGER();
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_TTS_APP_AUDIO_AUDIOFACTORY_HPP
