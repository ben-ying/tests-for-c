//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/IAudioSourceListener.hpp
/// @brief Definition of class IAudioSourceListener
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCELISTENER_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCELISTENER_HPP

#include <string>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Listeer for callbacks from IAudioSource.
 */
class IAudioSourceListener
{
public:
    IAudioSourceListener(){}
    virtual ~IAudioSourceListener(){}

    /// Indicates that output via a given channel has been started
    virtual void started() = 0;

    /// Requests an active connection to stop playback.
    virtual void onAudioConnectionMuted() = 0;

    /// Indicates that output via a given channel has been stopped
    virtual void stopped() = 0;

    /// Indicates that output via a given channel has been paused
    virtual void paused() = 0;

    /// Indicates that channel management failed to start output via a given channel
    virtual void startFailed() = 0;

    /// Indicates that channel management failed to stop output via a given channel
    virtual void stopFailed() = 0;

    /**
     * @brief Indicates that the status of the Source for a given channel.
     * @param channelName Audio Channel Id
     * @param available indicates if the source is available.
     */
    virtual void onSourceAvailabilityChanged(const std::string& channelName, const bool available) = 0;

};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCELISTENER_HPP
