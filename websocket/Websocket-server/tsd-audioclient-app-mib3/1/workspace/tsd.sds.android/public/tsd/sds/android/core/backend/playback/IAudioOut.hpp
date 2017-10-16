//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/core/backend/playback/IAudioOut.hpp
/// @brief Definition of class IAudioOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_CORE_BACKEND_PLAYBACK_IAUDIOOUT_HPP
#define TSD_SDS_ANDROID_CORE_BACKEND_PLAYBACK_IAUDIOOUT_HPP

#include <tsd/common/types/typedef.hpp>

namespace tsd { namespace sds { namespace android { namespace core { namespace backend { namespace playback {

/**
 * @brief Has to be implemented and provided by the client. Used to pass synthesized audio-samples to the client.
 * Before using a created IAudioOut instance, one has to call initialize() on the instance. Otherwise it is not
 * guaranteed, that the object will behave as expected.
 */
class IAudioOut
{
public:

    IAudioOut(){}
    virtual ~IAudioOut(){}

    /**
     * @brief writeSamples Called as soon as playback for the corresponding prompt was started.
     * The number of samples passed to this method never exceeds the number returned by getMaxNumberOfSamplesPerWrite().
     * If getMaxNumberOfSamplesPerWrite() returns 0 the core tries always to write all currently available samples.
     * The implementation has to be thread-safe, if multiple players shall use the instance.
     * @param[in] samples the samples, that shall be written.
     * @param[in] numOfSamples the length of the array given in @p samples.
     * @return the actual number of bytes written.
     */
    virtual size_t writeSamples(void* samples, size_t numOfSamples) = 0;

    /**
     * @brief finalize finalizes the instance. After calling this method, playback is no longer possible, until start() get's called.
     */
    virtual void finalize() = 0;

    /**
     * @brief start Starts playback.
     * After calling start() calls to write() write samples to the actual destination.
     */
    virtual void start() = 0;

    /**
     * @brief isStarted Returns true if start() has been called before, false otherwise.
     */
    virtual bool isStarted() const = 0;

    /**
     * @brief Called when the AudioFactory gets source availability callback.
     */
    virtual void initialize() = 0;

};

} // playback
} // backend
} // core
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_CORE_BACKEND_PLAYBACK_IAUDIOOUT_HPP
