//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/rsiapi/ISourceControl.hpp
/// @brief Definition of class ISourceControl
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////



#ifndef TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_ISOURCECONTROL_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_ISOURCECONTROL_HPP

#include "ApiSwitch.hpp"

#ifndef USE_INTERNAL_AUDIOMGR_API
#include <tsd/audio/sourcecontrol/mib3/api/ISourceControl.hpp>
namespace rsiapi = tsd::audio::sourcecontrol::mib3::api;

#endif

#ifdef USE_INTERNAL_AUDIOMGR_API


#include "IAudioConnectionsListener.hpp"//change by greg
#include "ISourceListener.hpp"

#include <memory>
#include <string>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio { namespace rsiapi {

   /**
    * @brief Interface for a source control object. Refer to the factory method below for creation and additional information on usage.
    */
   class ISourceControl
   {
      public:
         /** D'tor */
         virtual ~ISourceControl() {}

         /**
          * @brief Needs to be called before the object is functional. Returns true if RSI subscriptions on resource and element level
          *        succeeded. If the source object is already available, the listener will be called.
          *
          * @pre It is neccessary to have the Audiomanager running, otherwise this method will fail.
          *
          * @note Method is synchronous at the moment!
          *
          * @return If the subscriptions succeeded, returns true, false otherwise.
          */
         virtual bool connect() = 0;

         /**
          * @brief Tries to change the playstatus.
          *
          * @pre Successful call to connect().
          *
          * @note Method is synchronous at the moment!
          *
          * @param newStatus  New status to set
          * @return True on success (listener will be called), false otherwise
          */
         virtual bool changePlayStatus(const PlayStatus newStatus) = 0;

         /**
          * @brief Convenience method to retrieve the current play status.
          * @return  The last known playStatus
          */
         virtual PlayStatus getCurrentPlayStatus() const = 0;

         /**
          * @brief Convenience method to retrieve the current audio status.
          * @return  The last known audioStatus
          */
         virtual AudioStatus getCurrentAudioStatus() const = 0;

         /**
          * @brief Convenience method to retrieve the current source availability
          * @return  The last known source availability
          */
         virtual bool getCurrentSourceAvailabilitiy() const = 0;

         /**
          * @brief Call to post a connection status change
          * @param connectionName   Name of the connection, must be a valid connection for this source object
          * @param newStatus        Status to change to (paused is not allowed)
          * @return  true, if the POST was successful, false otherwise
          *
          * @note It is not allowed to change a connection to PAUSED status, so this will be rejected by the client
          */
         virtual bool changeConnectionStatus(const std::string& connectionName, const ConnectionStatus newStatus) = 0;

         /**
          * @brief Convenience method to retrieve the current list of valid connections for this source
          * @return The list
          */
         virtual const std::vector<std::string> getCurrentAudioConnections() const = 0;
   };

   /**
    * @brief Factory method to generate a source control object
    * @param sourceName          The name of the source to control (must be set according to the Audiomanagement)
    * @param sourceListener      The listener to get events dispatched to
    * @param connectionListener  Optional: a listener for connection related events
    * @return  A pointer to the control object.
    *
    * @note If you provide an invalid sourceName, a control object will still be created (but won't work). So make sure to use only the
    *       name valid for your source.
    */
   std::unique_ptr<ISourceControl> createSourceControl(
      const std::string& sourceName,
      ISourceListener* sourceListener,
      IAudioConnectionsListener* connectionListener = nullptr,
      const std::string& ipAddress = "127.0.0.1"
   );

} // rsiapi
} // audio
} // app
} // android
} // sds
} // tsd

#endif


#endif // TSD_SDS_TTS_APP_AUDIO_RSIAPI_ISOURCECONTROL_HPP

