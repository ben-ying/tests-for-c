//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/AudioConnectionDescription.hpp
/// @brief Definition of class AudioConnectionDescription
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_AUDIOCONNECTIONDESCRIPTION_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_AUDIOCONNECTIONDESCRIPTION_HPP

#include <string>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Audio connection description.
 * Data is used to configure the core's audio connection management.
 * See also: Mapping from connection ID to audio channel in configuration data.
 */
struct AudioConnectionDescription
{
   /**
    * @brief Audio connection UUID.
    */
   std::string m_AudioConnectionUuId;

   /**
    * @brief Audio channel ID. This channel will be used for the given connection.
    */
   std::string m_AudioChannelId;
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_TTS_APP_AUDIO_AUDIOCONNECTIONDESCRIPTION_HPP
