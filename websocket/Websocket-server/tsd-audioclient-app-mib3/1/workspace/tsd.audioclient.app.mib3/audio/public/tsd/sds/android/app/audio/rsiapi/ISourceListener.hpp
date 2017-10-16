//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/rsiapi/ISourceListener.hpp
/// @brief Definition of class ISourceListener
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_ISOURCELISTENER_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_ISOURCELISTENER_HPP

#include "ApiSwitch.hpp"

#ifdef USE_INTERNAL_AUDIOMGR_API

namespace tsd { namespace sds { namespace android { namespace app { namespace audio { namespace rsiapi {

   /**
    * API-USAGE
    *
    * The source objects are part of the Audiomanagement RSI. This API is a C++ convenience wrapper around these objects, please use it
    * instead of using raw RSI yourself.
    *
    * The AUDIOSTATUS property signals the source client, if a connection is muted, waiting to be faded or already faded in. The
    * Audiomanager listens to changes of the source-controlled PLAYSTATUS property.
    *
    * If a source can pause/halt its playback (e.g. a media player), PLAYSTATUS=IDLE means the player is paused, PLAYSTATUS=PLAYING means
    * the source is already playing. In the Audiomanagements READYTOFADE state it waits for the source to switch to PLAYING before the
    * Audiomanager fades in with the ramp. As soon as the fading is completed, the Audiomanager sets ADUIOSTATUS=FADEDIN.
    *
    * Depending on the needs of your source logic, you have to set PLAYSTATUS=PLAYING accordingly. If you want your source to be faded in,
    * start delivering samples as soon as you switch to PLAYING state. If you want to wait until the fading is over, you set your source to
    * PLAYING but wait for the Audiomanager to set AUDIOSTATUS=FADEDIN, before you start delivering samples to the Audiorouter client API.
    */

   /**
    * @brief Enum for the audioStatus property
    */
   enum AudioStatus
   {
      AudioStatus_undefined,
      AudioStatus_muted,
      AudioStatus_readyToFade,
      AudioStatus_fadedIn
   };

   /**
    * @brief Enum for the playStatus property
    */
   enum PlayStatus
   {
      PlayStatus_undefined,
      PlayStatus_idle,
      PlayStatus_playing
   };

   /**
    * @brief Interface class that gets source updates dispatched to
    */
   class ISourceListener
   {
      public:
         /** D'tor */
         virtual ~ISourceListener() {}

         /**
          * @brief Signals availability of the source object.
          * @param available  True, if available.
          */
         virtual void onSourceAvailabilityChanged(bool available) = 0;

         /**
          * @brief Signals a change of the audioStatus property. Will be triggered by the Audiomanager.
          * @param newStatus  The new audioStatus
          */
         virtual void onAudioStatusChanged(const AudioStatus newStatus) = 0;

         /**
          * @brief Signals a change of the playStatus property. Convenience method, will be triggered by the Source
          *        (@see ISourceControl::changePlayStatus()).
          * @param newStatus  The new playStatus
          */
         virtual void onPlayStatusChanged(const PlayStatus newStatus) = 0;
   };

} // rsiapi
} // audio
} // app
} // android
} // sds
} // tsd

#endif

#endif // TSD_SDS_TTS_APP_AUDIO_RSIAPI_ISOURCELISTENER_HPP
