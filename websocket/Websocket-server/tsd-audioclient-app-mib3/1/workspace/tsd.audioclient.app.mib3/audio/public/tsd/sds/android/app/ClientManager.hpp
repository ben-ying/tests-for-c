/*
 * ClientManager.hpp
 *
 *  Created on: Oct 9, 2017
 *      Author: sunjianguang
 */

#ifndef TSD_AUDIOCLIENT_APP_MIB3_1_WORKSPACE_TSD_AUDIOCLIENT_APP_MIB3_AUDIO_PUBLIC_TSD_SDS_ANDROID_APP_CLIENTMANAGER_HPP_
#define TSD_AUDIOCLIENT_APP_MIB3_1_WORKSPACE_TSD_AUDIOCLIENT_APP_MIB3_AUDIO_PUBLIC_TSD_SDS_ANDROID_APP_CLIENTMANAGER_HPP_
#include <tsd/audio/audiorouter/client/ISession.hpp>
#include <tsd/common/types/SmartPtr.hpp>
#include <tsd/sds/utils/logging/Logger.hpp>
#include "tsd/sds/android/app/audio/AudioFactory.hpp"
#include "tsd/sds/android/app/WorkerThreadPool.hpp"
#include "tsd/sds/android/app/config/AppConfiguration.hpp"
namespace tsd { namespace sds { namespace android { namespace app {
class ClientManager:public tsd::sds::utils::threads::IExceptionHandler {
public:
    ClientManager();
    virtual ~ClientManager();
    /**init Audio control RSI client.
     */
    tsd::common::types::SmartPtr<audio::IAudioFactory> &getAudioFactory();
private:
    /**
     * @brief Returns an audio session.
     * @return Audio session or - in case of an error - an invalid pointer.
     */
    tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession>& getAudioSession();

    /**
     * @brief @see tsd::sds::utils::threads::IExceptionHandler
     */
    virtual void handle(tsd::common::errors::BaseException& exception) override;

    /**
     * @brief @see tsd::sds::utils::threads::IExceptionHandler
     */
    virtual void handle(std::exception& exception) override;

    /**
     * @brief @see tsd::sds::utils::threads::IExceptionHandler
     */
    virtual void handle() override;

    /// Audio session for communication with audio management
    tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession> m_AudioSession;
    /// contains information about connection mappings
    tsd::common::types::SmartPtr<tsd::sds::android::app::audio::IAudioFactory> m_AudioFactory;
    /// Contains the worker threads used by the application.
    WorkerThreadPool m_WorkerThreadPool;
    /// path to the configuration file used by the application.
    std::string m_ConfigurationFile;
    /// @see config::AppConfiguration
    config::AppConfiguration m_Configuration;
    /// used to configure the audio factory to use audiomanagement or a mock implementation
    bool m_UseAudioManagement;
    DECL_LOGGER();
};

} // app
} // android
} // sds
} // tsd

#endif /* TSD_AUDIOCLIENT_APP_MIB3_1_WORKSPACE_TSD_AUDIOCLIENT_APP_MIB3_AUDIO_PUBLIC_TSD_SDS_ANDROID_APP_CLIENTMANAGER_HPP_ */
