//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioSource.hpp
/// @brief Definition of class AudioSource
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_AUDIOSOURCE_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_AUDIOSOURCE_HPP

#include <tsd/sds/utils/logging/Logger.hpp>
#include <tsd/sds/utils/threads/SimpleWorkerThread.hpp>
#include <tsd/common/system/Mutex.hpp>
#include <list>
#include <queue>
#include <functional>
#include "IAudioSource.hpp"
#include "IAudioSourceListener.hpp"
#include "tsd/sds/android/app/audio/rsiapi/ISourceControl.hpp"


namespace tsd { namespace sds { namespace android { namespace app { namespace audio {


/**
 * @brief The SyncAudioOut class Implementation of core::prompt::playback::IAudioOut.
 * Writes samples to an instance of tsd::audio::audiorouter::client::IPlayback().
 * For listener registration @see tsd::sds::android::app::audio::AudioOutBase.
 */
class AudioSource : public IAudioSource,
      public rsiapi::IAudioConnectionsListener,
      public rsiapi::ISourceListener

{
public:

   /**
    * @brief Construtor.
    * @param[in] workerThread All calls are executed in this thread.
    * @param[in] audioSourceListener AudioFactory is the listener. Called onSourceAvailabilityChanged.
    * @param[in] channelId the id of the AudioSource
    * @param[in] useAudioManagement
    */
   AudioSource(tsd::common::types::SmartPtr<tsd::sds::utils::threads::SimpleWorkerThread> workerThread,
               IAudioSourceListener* audioSourceListener,
                const std::string& channelId,
                bool useAudioManagement = true);

   /**
    * @brief Destructor.
    */
   virtual ~AudioSource();

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioSource::registerListener()
    */
    virtual bool registerListener(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener) override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioSource::cancelListenerRegistration()
    */
   virtual void cancelListenerRegistration(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener) override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioSource::start()
    */
   virtual void start(const std::string& connectionName) override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioSource::stop()
    */
   virtual void stop(const std::string& connectionName) override;

   /**
    * @brief Initializes the instance. Has to be called before the instance is useable.
    * @return the source control instance used by this instance.
    */
   virtual tsd::common::types::SmartPtr<tsd::sds::android::app::audio::rsiapi::ISourceControl> initialize();// change by greg
private:

   /// channel listener description
   typedef std::pair<const std::string, tsd::sds::android::app::audio::IAudioSourceListener*> ListenerDescription;

   /// Used to issue the connect command to the channel.
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> m_WorkerThread;

   IAudioSourceListener* m_AudioSourceListener;

   /// Audio channel ID
   const std::string m_Source;

   /// Container for all registered listeners
   std::list<ListenerDescription> m_Listeners;

   /// Controls an audio channel
   tsd::common::types::SmartPtr<rsiapi::ISourceControl> m_SourceControl;

   /// 'true': Channel management is used. 'false': No channel management.
   bool m_UseAudioManagement;

   /// contains the id of the requested connection
   std::string m_RequestedConnection;

   /// contains subsequent connection requests. A connection request will be enqueued, if another request is pending.
   std::queue<std::string> m_RequestQueue;

   /// contains the id of the active connection.
   std::string m_ActiveConnection;

   /// the current audio status of this instance.
   rsiapi::AudioStatus m_AudioStatus;

   /// the current play status of this instance.
   rsiapi::PlayStatus m_PlayStatus;

   /// contains the connection states for all connections.
   std::map<std::string, rsiapi::ConnectionStatus> m_ConnectionStates;

   tsd::common::system::Mutex m_Mutex;


   // inherited from ISourceListener

   /**
    * @brief @see rsiapi::ISourceListener
    */
   virtual void onSourceAvailabilityChanged(bool available) override;

   /**
    * @brief @see rsiapi::ISourceListener
    */
   virtual void onAudioStatusChanged(const rsiapi::AudioStatus newStatus) override;

   /**
    * @brief @see rsiapi::ISourceListener
    */
   virtual void onPlayStatusChanged(const rsiapi::PlayStatus newStatus) override;


   // inherited from IAudioConnectionListener
   /**
   * @brief @see rsiapi::IAudioConnectionsListener
   */
   virtual void onAudioConnectionListChanged(const std::vector<std::string>&) override;

   /**
   * @brief @see rsiapi::IAudioConnectionsListener
   */
   virtual void onAudioConnectionStatusChanged(const std::string& connectionName, const rsiapi::ConnectionStatus newStatus) override;

   void processStart(const std::string& connectionName);
   void processStop(const std::string& connectionName);

   void processAudioStateChange(const rsiapi::AudioStatus newStatus);
   void processPlayStateChange(const rsiapi::PlayStatus newStatus);
   void processSourceAvailabilityChange(const bool available);
//   void processConnectionListChange(const rsiapi::AudioStatus newStatus);
   void processConenctionStateChange(const std::string& connectionName, const rsiapi::ConnectionStatus newStatus);


   void doCallback(const std::string& connectionName, std::function<void(ListenerDescription&)> callback);

   DECL_LOGGER();
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_TTS_APP_AUDIO_AUDIOSOURCE_HPP
