//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/IAudioFactory.hpp
/// @brief Definition of class IAudioFactory
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_IAUDIOFACTORY_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_IAUDIOFACTORY_HPP

#include <tsd/sds/android/core/backend/playback/IAudioOut.hpp>
#include <tsd/common/types/SmartPtr.hpp>
#include <tsd/common/types/typedef.hpp>
#include <set>
#include <string>
#include "tsd/sds/android/app/audio/IAudioConnection.hpp"
#include "tsd/sds/android/app/audio/IAudioSource.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Provides access to audio related objects and values.
 */
class IAudioFactory
{
public:
    IAudioFactory(){}
    virtual ~IAudioFactory(){}

    /**
     * @brief Returns a set of all known connection ids.
     * @return a set of all known connection ids.
     */
    virtual std::set<std::string>& getConnectionIds() = 0;

    /**
     * @brief Returns a set of all known sourc ids.
     * @return a set of all known source ids
     */
    virtual std::set<std::string>& getAudioSourceIds() = 0;

    /**
     * @brief Returns the IAudioConnection instance for the connection given in @p connection.
     * @param[in] connection the name of the connection.
     * @return the IAudioConnection instance for the connection given in @p connection. May return null, if the connection instance does not exist.
     */
    virtual tsd::common::types::SmartPtr<audio::IAudioConnection> getAudioConnection(const std::string& connection) = 0;

    /**
     * @brief Returns the source id for the connection given in @p connection.
     * @param[in] connection the name of the connection.
     * @return the source id for the connection given in @p connection.
     */
    virtual const std::string& getAudioSourceIdForAudioConnection(const std::string& audioConnection) = 0;

    /**
     * @brief Returns the IAudioOut instance for the source given in @p sourceId.
     * @param[in] sourceId the name of the audio source.
     * @return the IAudioOut instance for the source given in @p sourceId. May return null, if the IAudioOut instance does not exist.
     */
    virtual tsd::common::types::SmartPtr<core::backend::playback::IAudioOut> getAudioOut(const std::string& sourceId) = 0;





};


} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_IAUDIOFACTORY_HPP
