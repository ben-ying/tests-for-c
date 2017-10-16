//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/rsiapi/RSISourceControl.hpp
/// @brief Definition of class RSISourceControl
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_TTS_APP_AUDIO_RSIAPI_RSISOURCECONTROL_HPP
#define TSD_SDS_TTS_APP_AUDIO_RSIAPI_RSISOURCECONTROL_HPP

#include "tsd/sds/android/app/audio/rsiapi/ApiSwitch.hpp"

#ifdef USE_INTERNAL_AUDIOMGR_API


#include <tsd/sds/android/app/audio/rsiapi/ISourceControl.hpp>
#include <tsd/sds/utils/logging/Logger.hpp>
#include <tsd/common/system/Mutex.hpp>
#include <tsd/common/types/typedef.hpp>
#include <tsd/rsi/client/rsiclient.hpp>
#include <tsd/dataformats/data/value.hpp>
#include <tsd/dataformats/data/array.hpp>
#include <tsd/dataformats/data/object.hpp>
#include <tsd/sds/utils/threads/SimpleWorkerThread.hpp>
#include <set>
#include <string>

namespace rsidata = tsd::dataformats::data;
namespace rsiclient = tsd::rsi::client;


namespace tsd { namespace sds { namespace android { namespace app { namespace audio { namespace rsiapi {



   /**
    * @brief RSI implementation of a source control class
    */
   class RSISourceControl : public ISourceControl,
                            public tsd::rsi::client::EventHandler
   {
      public:
         /**
          * @brief C'tor
          * @param sourceName Name of the source this object controls. Must be valid according to the audiomanagement
          * @param listener   Listener object to dispatch updates to
          */
         RSISourceControl(const std::string& sourceName,
                          ISourceListener* sourceListener,
                          IAudioConnectionsListener* audioConnectionsListener,
                          const std::string& ipAddress);

         /** D'tor */
         virtual ~RSISourceControl() override;

         /** see base class ISourceControl */
         virtual bool connect() override;

         /** see base class ISourceControl */
         virtual bool changePlayStatus(const PlayStatus newStatus) override;

         /** see base class ISourceControl */
         virtual PlayStatus getCurrentPlayStatus() const override;

         /** see base class ISourceControl */
         virtual AudioStatus getCurrentAudioStatus() const override;

         /** see base class ISourceControl */
         virtual bool getCurrentSourceAvailabilitiy() const override;

         /** see base class ISourceControl */
         virtual bool changeConnectionStatus(const std::string& connectionName, const ConnectionStatus newStatus) override;

         /** see base class ISourceControl */
         virtual const std::vector<std::string> getCurrentAudioConnections() const override;

         /** @brief Overridden event receive method from RSI client, see base class for details */
         virtual void handleEvent( const tsd::rsi::client::RequestState& state, const tsd::rsi::client::Response& response ) override;

      private:

         mutable tsd::common::system::Mutex m_Mutex; ///< Mutex to secure internals

         ISourceListener* m_SourceListener; ///< The source listener to dispatch events to
         IAudioConnectionsListener* m_AudioConnectionsListener; ///< The audioconnection listener to dispatch events to

         const std::string m_SourceName; ///< The name of the source to control
         const std::string m_SourceUUID; ///< The UUID of the source

         std::unique_ptr<tsd::rsi::client::IRsiClient> m_Client; ///< The RSI client instance

         int32_t m_SourceResourceSubscriptionId; ///< Subscription id (used for unsubscribe only)
         const int32_t m_SourceResourceSubscriptionUserId; ///< Subscription id to distinguish events

         int32_t m_SourceElementSubscriptionId; ///< Subscription id (used for unsubscribe only)
         const int32_t m_SourceElementSubscriptionUserId; ///< Subscription id to distinguish events

         int32_t m_ConnectionResourceSubscriptionId; ///< Subscription id (used for unsubscribe only)
         const int32_t m_ConnectionResourceSubscriptionUserId; ///< Subscription id to distinguish events

         const int32_t m_ConnectionElementSubscriptionUserId; ///< Subscription id to distinguish events

         /**
          * @brief Helper struct to manage connections
          */
         struct Connection
         {
            std::string m_ConnectionName; ///< Name of the connection
            std::string m_ConnectionUUID; ///< UUID of the connection
            int32_t m_SubscriptionId; ///< Subscription id, if this connection was subscribed (only for "our" connections)
         };
         std::set<Connection,bool(*)(const Connection&, const Connection&)> m_CurrentKnownConnections; ///< The list of all known connections

         AudioStatus m_CurrentAudioStatus; ///< Last known audio status
         PlayStatus m_CurrentPlayStatus; ///< Last known play status
         bool m_CurrentSourceAvailabilitiy; ///< Last known source availability

         DECL_LOGGER();


         tsd::sds::utils::threads::SimpleWorkerThread m_WorkerThread;

         /**
          * @brief Internal helper method to rip through RSI element data, calls listener if approppiate
          * @param data Pointer to the data
          * @return True, if a valid RSI object was parsed, false otherwise
          */
         bool handleSourceElementData(tsd::common::types::SmartPtr<dataformats::data::Object> dataObject);

         /**
          * @brief Internal helper method to rip through RSI resource data, calls listener if approppiate
          * @param data Pointer to the data
          * @return True, if a valid RSI resource array was parsed, false otherwise
          */
         void handleSourceResourceData(tsd::common::types::SmartPtr<dataformats::data::Array> array);

         /**
          * @brief Internal helper method to rip through RSI resource data, calls listener if approppiate
          * @param data Pointer to the data
          * @return True, if a valid RSI resource array was parsed, false otherwise
          */
         void handleConnectionElementData(tsd::common::types::SmartPtr<dataformats::data::Object> connData);

         /**
          * @brief Internal helper method to rip through RSI resource data, calls listener if approppiate
          * @param data Pointer to the data
          * @return True, if a valid RSI resource array was parsed, false otherwise
          */
         void handleConnectionResourceData(tsd::common::types::SmartPtr<dataformats::data::Array> array);

         /**
          * @brief Helper method to post subscriptions
          * @param subscriptionUserId  The user id that is handed to the subscriptions for the callback
          * @param resourceName        The name of the resource to subscribe to
          * @param elementUUID         The UUID of the element to subscribe for (if empty, a subscription for the resource is done instead)
          * @return The subscription id if all is ok, INVALID_SUBSCRIPTION_ID if something went wrong
          */
         int32_t subscribe(const int32_t subscriptionUserId, const std::string& resourceName, const std::string& elementUUID = "");

         /**
          * @brief Helper method to get data from the server
          * @param response      Data container for the response data
          * @param resourceName  The name of the resource to get
          * @param elementUUID   The UUID of the element to get (if empty, a get on resource level is done instead)
          * @return True, if the get was successfull, false otherwise
          */
         bool get(tsd::rsi::client::Response& response, const std::string& resourceName, const std::string& elementUUID = "");

         /**
          * @brief Helper method to post data to the server
          * @param dataToPost    Data to post
          * @param resourceName  The name of the resource to post to
          * @param elementUUID   The UUID of the element to post to
          * @return True, if the post was successfull, false otherwise
          */
         bool post(std::unique_ptr<tsd::dataformats::data::Value> dataToPost, const std::string& resourceName, const std::string& uuid);

         /**
          * @brief Helper method to generate the list of "valid" connections out of the list of all known connections
          * @return The list of valid connection names
          */
         const std::vector<std::string> generateConnectionList() const;
   };

} // rsiapi
} // audio
} // app
} // android
} // sds
} // tsd

#endif

#endif // TSD_SDS_TTS_APP_AUDIO_RSIAPI_RSISOURCECONTROL_HPP

