//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/rsiapi/IAudioConnectionsListener.hpp
/// @brief Definition of class IAudioConnectionsListener
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_IAUDIOCONNECTIONSLISTENER_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_RSIAPI_IAUDIOCONNECTIONSLISTENER_HPP

#include "tsd/sds/android/app/audio/rsiapi/ApiSwitch.hpp"

#ifndef USE_INTERNAL_AUDIOMGR_API
#include <tsd/audio/sourcecontrol/mib3/api/IAudioConnectionsListener.hpp>
namespace rsiapi = tsd::audio::sourcecontrol::mib3::api;

#endif
#ifdef USE_INTERNAL_AUDIOMGR_API

#include <string>
#include <vector>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio { namespace rsiapi {

   /**
   * @brief Enum describing all source-related connection status
   * @todo do we need PAUSED here?
   */
   enum ConnectionStatus
   {
      ConnectionStatus_started,
      ConnectionStatus_stopped,
      ConnectionStatus_paused
   };

   /**
    * @brief Listener interface for RSI audioconnection status changes
    */
   class IAudioConnectionsListener
   {
      public:

         /** D'tor */
         virtual ~IAudioConnectionsListener() {}

         /**
         * @brief Callback for a changed connection list (the names) (regarding the source the listener was registered for)
         * @param newList   The new connection names list
         */
         virtual void onAudioConnectionListChanged(const std::vector<std::string>& newList) = 0;

         /**
         * @brief Callback for status changes on audio RSI connections (regarding the source the listener was registered for)
         * @param connectionName    The name of the connection
         * @param newStatus         The new status
         */
         virtual void onAudioConnectionStatusChanged(const std::string& connectionName, const ConnectionStatus newStatus) = 0;
   };


} // rsiapi
} // audio
} // app
} // android
} // sds
} // tsd

#endif

#endif // TSD_SDS_TTS_APP_AUDIO_RSIAPI_IAUDIOCONNECTIONSLISTENER_HPP

