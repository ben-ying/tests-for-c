//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/IAudioSource.hpp
/// @brief Definition of class IAudioSource
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCE_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCE_HPP

#include <string>
#include "tsd/sds/android/app/audio/IAudioSourceListener.hpp"

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief This class provides audio channel management functions.
 */
class IAudioSource
{
public:
   /// Constructor.
   IAudioSource() {}

   /// Destructor.
   virtual ~IAudioSource() {}

   /**
    * @brief Register a listener for connection status updates.
    * @param[in] connectionName Name of the connection whose status updates are requested.
    * @param[in] listener This listener will be registered.
    * @return 'true' if registration was successful, 'false' otherwise.
    */
    virtual bool registerListener(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener) = 0;

   /**
    * @brief Cancel registration of a listener.
    * @param[in] connectionName Name of the connection whose status updates are no longer requested.
    * @param[in] listener This listener's registration will be cancelled.
    */
   virtual void cancelListenerRegistration(const std::string& connectionName, tsd::sds::android::app::audio::IAudioSourceListener* listener) = 0;

   /**
    * @brief Prepars playback for the given connection.
    * After calling this method and receiving the 'started' callback in the corresponding listener, one may start writing
    * to the corresponding audio out instance.
    * @param[in] connectionName name of the connection, that shall be started.
    */
   virtual void start(const std::string& connectionName) = 0;

   /**
    * @brief Stops the given audio connection if it is currently started.
    * Before sending the callback, that the connection is actually stopped, the instance may send a requestStopPlayback to the corresponding listener(s).
    * If this callback is sent, the instance expects a second call to stop(), then the callback 'stopped' will be called.
    * @param[in] connectionName name of the connection, that shall be stopped.
    */
   virtual void stop(const std::string& connectionName) = 0;
};

} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_ANDROID_APP_AUDIO_IAUDIOSOURCE_HPP
