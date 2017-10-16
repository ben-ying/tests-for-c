//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/WaveOut.cpp
/// @brief Implementation of class WaveOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/WaveOut.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef TARGET_OS_POSIX_QNX
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <tsd/common/system/Thread.hpp>

#endif

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

DEF_LOGGER(audio::WaveOut);

WaveOut::WaveOut()
   : tsd::sds::android::core::backend::playback::IAudioOut()
   , mSampleRate(22050)
   , mSampleChannels(1)
   , pcm()
   , pcmDeviceName("default")
   , pcm_handle(nullptr)
   , params(nullptr)
   , frames()
   , m_BytesPerWrite()
   , format(SND_PCM_FORMAT_S16_LE)
{

}

WaveOut::~WaveOut()
{

}

size_t WaveOut::writeSamples(void *samples, size_t length)
{
   size_t bytesWritten = 0;
   while(bytesWritten < length) {
       int bytesToWrite = static_cast<int>(m_BytesPerWrite);

       if(bytesWritten + m_BytesPerWrite > length)
       {
           bytesToWrite = static_cast<int>(length - bytesWritten);
       }
       snd_pcm_uframes_t framesToWrite = bytesToWrite / 2;
       if ((pcm = static_cast<int>(snd_pcm_writei(pcm_handle, static_cast<char*>(samples) + bytesWritten, framesToWrite))) == -EPIPE) {

           snd_pcm_prepare(pcm_handle);
       } else if (pcm < 0) {
           printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
       }
       bytesWritten += bytesToWrite;
   }
   return bytesWritten;
}

void WaveOut::finalize()
{
   m_Log.logInfo("finalize");
   snd_pcm_drain(pcm_handle);
   snd_pcm_close(pcm_handle);
}

void WaveOut::start()
{
   m_Log.logInfo("start");
   pcm = snd_pcm_open(&pcm_handle, pcmDeviceName, SND_PCM_STREAM_PLAYBACK, 0);

   /* Open the PCM device in playback mode */
   if (pcm < 0)
   {   printf("ERROR: Can't open \"%s\" PCM device. %s\n",
              pcmDeviceName, snd_strerror(pcm));
   }

   /* Allocate parameters object and fill it with default values*/
   snd_pcm_hw_params_alloca(&params);

   snd_pcm_hw_params_any(pcm_handle, params);
   pcm = snd_pcm_hw_params_set_access(pcm_handle, params,SND_PCM_ACCESS_RW_INTERLEAVED);
   /* Set parameters */
   if ( pcm < 0)
   {
       printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));
   }

   pcm = snd_pcm_hw_params_set_format(pcm_handle, params, format);

   if (pcm < 0)
   {
   printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
   }

   pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, mSampleChannels);
   if (pcm < 0)
   {
       printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
   }

   pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &mSampleRate, 0);
   if (pcm < 0)
   {
       printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
   }

   /* Write parameters */

   pcm = snd_pcm_hw_params(pcm_handle, params);
   if (pcm < 0)
   {
       printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));
   }
   /* Allocate buffer to hold single period */
   snd_pcm_hw_params_get_period_size(params, &frames, 0);

   m_BytesPerWrite = frames * mSampleChannels * 2 /* 2 -> sample size */;
   snd_pcm_prepare(pcm_handle);

   m_Log.logInfo(__func__) << "samples per write: " << m_BytesPerWrite;

}



} // audio
} // app
} // android
} // sds
} // tsd

