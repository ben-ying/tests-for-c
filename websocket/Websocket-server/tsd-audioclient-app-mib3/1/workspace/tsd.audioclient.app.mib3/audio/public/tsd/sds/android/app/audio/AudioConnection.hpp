//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioConnection.hpp
/// @brief Definition of class AudioConnection
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_AUDIOCONNECTION_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_AUDIOCONNECTION_HPP


#include <tsd/sds/utils/threads/SimpleWorkerThread.hpp>
#include <tsd/sds/utils/logging/Logger.hpp>
#include <tsd/common/types/SmartPtr.hpp>
#include <functional>
#include <set>
#include "tsd/sds/android/app/audio/AudioSource.hpp"
#include "tsd/sds/android/app/audio/IAudioConnection.hpp"
#include "tsd/sds/android/app/audio/IAudioSourceListener.hpp"
#include "tsd/sds/android/app/audio/rsiapi/IAudioConnectionsListener.hpp"
#include "tsd/sds/android/app/audio/rsiapi/ISourceListener.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief manages one audio connections
 */

class AudioConnection : public IAudioConnection,
                        public IAudioSourceListener

{
public:
   /**
    * @brief Constructor.
    * @param[in] audioOut Instance of IAudioOut for audio sample output.
    * @param[in] connectionId The id of the audio-out connection.
    * @param[in] channelId the id of the audio-out channel used to communicate to the audio router.
    */
   AudioConnection(tsd::common::types::SmartPtr<IAudioSource> audioChannel,
                   const std::string& connectionId,
                   const std::string& channelId);

   /**
    * @brief Destructor.
    */
   virtual ~AudioConnection();

   /**
    * @brief Returns the connection ID
    */
   const std::string& getConnectionId();

   // ========== Inherited from IAudioConnection ==========

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioConnection::registerListener()
    */
   virtual bool registerListener(tsd::sds::android::app::audio::IAudioConnectionListener* listener) override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioConnection::cancelListenerRegistration()
     */
   virtual void cancelListenerRegistration(tsd::sds::android::app::audio::IAudioConnectionListener* listener) override;

   /**
   * @brief @see tsd::sds::android::app::audio::IAudioConnection::getId()
    */
   virtual const std::string& getId() override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioConnection::start()
    */
   virtual void start() override;

   /**
    * @brief @see tsd::sds::android::app::audio::IAudioConnection::stop()
    */
   virtual void stop() override;

   /**
    * @brief Initializes the instance. Has to be called before the instance is usable.
    */
   virtual void initialize();

private:

  /**
   * @see IAudioChannelListener
   */
//   virtual void onAudioConnectionStatusChanged(const rsiapi::ConnectionStatus newStatus) override;

   /**
    * @see IAudioChannelListener
    */
   virtual void started() override;


   /**
    * @see IAudioChannelListener
    */
   virtual void onAudioConnectionMuted() override;

   /**
    * @see IAudioChannelListener
    */
   virtual void stopped() override;

   /**
    * @see IAudioChannelListener
    */
   virtual void paused() override;

   /**
    * @see IAudioChannelListener
    */
   virtual void startFailed() override;

   /**
    * @see IAudioChannelListener
    */
   virtual void stopFailed() override;

   /**
    * @brief IAudioSourceListener
    */
   virtual void onSourceAvailabilityChanged(const std::string& /*channelName*/, const bool /*available*/) override {}
   void listenerOperation(std::function<void(IAudioConnectionListener*)> func);

   /// Instance of IAudioOut, used for audio sample output.
   tsd::common::types::SmartPtr<IAudioSource> m_AudioChannel;

   /// Container for listener pointers
   std::set<IAudioConnectionListener*> m_Listeners;

   /// Connection ID (corresponds to ISourceControl's 'connectionName').
   std::string m_ConnectionId;

   /// Audio channel ID
   std::string m_ChannelId;

   /// This mutex is used to prevent concurrent access to connection status attributes
   tsd::common::system::Mutex m_Mutex;

   DECL_LOGGER();
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_AUDIO_AUDIOCONTROL_HPP
