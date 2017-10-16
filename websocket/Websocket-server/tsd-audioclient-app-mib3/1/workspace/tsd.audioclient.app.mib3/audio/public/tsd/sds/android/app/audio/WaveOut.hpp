//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/WaveOut.hpp
/// @brief Definition of class WaveOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_WAVEOUT_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_WAVEOUT_HPP

#ifdef TARGET_OS_WIN32
#include <Windows.h>
#include <MMSystem.h>
#elif TARGET_OS_POSIX_QNX
#include <sys/asoundlib.h>
#include <gulliver.h>
#elif TARGET_OS_POSIX_LINUX
#include <alsa/asoundlib.h>
#endif
#include <string>
#include <tsd/sds/utils/logging/Logger.hpp>
#include <tsd/sds/android/core/backend/playback/IAudioOut.hpp>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Implementation of tsd::sds::android::core::backend::playback::IAudioOut
 * Writes samples directly to alsa.
 */
class WaveOut  : public tsd::sds::android::core::backend::playback::IAudioOut
{
public:
   WaveOut();
   ~WaveOut();

   /**
    * @brief @see tsd::sds::android::core::backend::playback::IAudioOut
    */
   virtual size_t writeSamples(void* samples, size_t length);

   /**
    * @brief @see tsd::sds::android::core::backend::playback::IAudioOut
    */
   virtual void finalize();

   /**
    * @brief @see tsd::sds::android::core::backend::playback::IAudioOut
    */
   virtual void start();

private:

   unsigned int mSampleRate;
   int     mSampleChannels;
   int pcm;

   const char* pcmDeviceName;

   snd_pcm_t *pcm_handle;
   snd_pcm_hw_params_t *params;
   snd_pcm_uframes_t frames;
   size_t m_BytesPerWrite;
   snd_pcm_format_t format;

   DECL_LOGGER();
};




} // audio
} // app
} // android
} // sds
} // tsd

#endif
