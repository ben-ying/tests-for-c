//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/IAudioConnection.hpp
/// @brief Definition of class IAudioConnection
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTION_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTION_HPP

#include <string>
#include "IAudioConnectionListener.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {


/**
 * @brief This class provides audio connection management functions.
 */
class IAudioConnection
{
public:
   /// Constructor.
   IAudioConnection() {}

   /// Destructor.
   virtual ~IAudioConnection() {}

   /**
    * @brief Register a listener for connection status updates.
    * @param[in] listener This listener will be registered.
    * @return 'true' if registration was successful, 'false' otherwise.
    */
   virtual bool registerListener(tsd::sds::android::app::audio::IAudioConnectionListener* listener) = 0;

   /**
     * @brief Cancel registration of a listener.
     * @param[in] listener This listener's registration will be cancelled.
     */
   virtual void cancelListenerRegistration(tsd::sds::android::app::audio::IAudioConnectionListener* listener) = 0;

   /**
    * @brief Returns the uuid of the underlying audioconnection.
    */
   virtual const std::string& getId() = 0;
   /**
     * @brief Prepare audio output
     */
   virtual void start() = 0;

   /**
    * @brief Stop the connection. Any registered listener will receive a callback, that it's playback shall be stopped.
    * If no listener is registered, the instance will asume, that no playback is running anymore.
    */
   virtual void stop() = 0;

};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_IAUDIOCONNECTION_HPP
