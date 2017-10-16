//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/SyncAudioOut.hpp
/// @brief Definition of class SyncAudioOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_SYNCAUDIOOUT_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_SYNCAUDIOOUT_HPP

#include <tsd/sds/android/core/backend/playback/IAudioOut.hpp>
#include <tsd/audio/audiorouter/client/ISession.hpp>
#include <tsd/audio/audiorouter/client/ChannelConfiguration.hpp>
#include <tsd/audio/audiorouter/client/IPlayback.hpp>
#include <tsd/sds/utils/logging/Logger.hpp>
#include "tsd/sds/android/app/audio/rsiapi/ISourceControl.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Implementation of core::prompt::playback::IAudioOut.
 * Writes samples to an instance of tsd::audio::audiorouter::client::IPlayback().
 * For listener registration @see tsd::sds::android::app::audio::AudioOutBase.
 * The using class may call initialize with a source control instance. If the source control instance
 * is not null, it will be used to set the play status in start() and finalize().
 */
class SyncAudioOut : public tsd::sds::android::core::backend::playback::IAudioOut
{
public:
   /**
    * @brief SyncAudioOut Constructor.
    * @param[in] channelId the id of the audio-out channel used to communicate to the audio router.
    * @param[in] session Instance of ISession for audio router communication.
    */
   SyncAudioOut(const std::string& channelId,
                tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession>& session);

   /**
    * @brief ~SyncAudioOut Destructor.
    */
   virtual ~SyncAudioOut();

   // ========== Inherited from IAudioOut ==========

   /**
    * @brief @see core::prompt::playback::IAudioOut::writeSamples()
    */
   virtual size_t writeSamples(void* data, size_t len) override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::finalize()
    */
   virtual void finalize() override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::start()
    */
   virtual void start() override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::istStarted()
    */
   virtual bool isStarted() const override;

   /**
    * @brief Initializes the instance with the given source control instance.
    * @param sourceControl the source control instance. If set, it will be used to set the play status.
    */
   virtual void initialize(tsd::common::types::SmartPtr<rsiapi::ISourceControl> sourceControl);

   /**
    * @brief core::prompt::playback::IAudioOut::initialize()
    */
   virtual void initialize() override;

private:
   /// audio channel configuration
   tsd::audio::audiorouter::client::ChannelConfiguration m_ChannelConfiguration;

   /// Audio route session object which is used to get a playback object
   tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession> m_Session;

   /// Audio router playback object used as synchronous audio sink
   tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::IPlayback> m_Playback;

   /// needed to set the playstatus.
   tsd::common::types::SmartPtr<rsiapi::ISourceControl> m_SourceControl;

   /// Audio channel ID
   std::string m_ChannelId;

   /// indicates, that the instance is ready to receive samples (a call to 'start()' was successful).
   bool m_Started;



   DECL_LOGGER();
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_SYNCAUDIOOUT_HPP
