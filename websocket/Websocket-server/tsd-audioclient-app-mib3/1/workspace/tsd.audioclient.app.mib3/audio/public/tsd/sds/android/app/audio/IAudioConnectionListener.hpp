//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/IAudioConnectionListener.hpp
/// @brief Definition of class IAudioConnectionListener
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTIONLISTENER_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTIONLISTENER_HPP

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

class IAudioConnectionListener
{
public:
    IAudioConnectionListener(){}
    virtual ~IAudioConnectionListener(){}

    /// Indicates that output via a given connection has been started
    virtual void audioConnectionStarted() = 0;

    /// Indicates that the audio connection has been muted.
    virtual void onAudioConnectionMuted() = 0;

    /// Indicates that output via a given connection has been stopped
    virtual void audioConnectionStopped() = 0;

    /// Indicates that output via a given connection has been paused
    virtual void audioConnectionPaused() = 0;

    /// Indicates that channel management failed to start output via a given connection
    virtual void audioConnectionStartFailed() = 0;

    /// Indicates that channel management failed to stop output via a given connection
    virtual void audioConnectionStopFailed() = 0;
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTIONLISTENER_HPP
