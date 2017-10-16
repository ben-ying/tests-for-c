//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/rsiapi/RSISourceControl.cpp
/// @brief Implementation of class RSISourceControl
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////


#include "tsd/sds/android/app/audio/rsiapi/ApiSwitch.hpp"

#ifdef USE_INTERNAL_AUDIOMGR_API

#include <tsd/sds/android/app/audio/rsiapi/RSISourceControl.hpp>
#include <tsd/common/checksums/sha1.hpp>
#include <tsd/common/system/MutexGuard.hpp>
#include <tsd/dataformats/data/array.hpp>
#include <tsd/dataformats/data/object.hpp>
#include <tsd/dataformats/data/string.hpp>
#include <tsd/dataformats/data/value.hpp>
#include <sstream>
#include <iomanip>

namespace rsidata = tsd::dataformats::data;
namespace rsiclient = tsd::rsi::client;

namespace tsd { namespace sds { namespace android { namespace app { namespace audio { namespace rsiapi {

DEF_LOGGER(RSISourceControl);

const int32_t AUDIOMANAGEMENT_PORT = 49253; // TODO remove if client supports that itself

const int32_t INVALID_SUBSCRIPTION_ID = -1; // arbitrary, TODO: does RSI generate negative subscription IDs?

const char RSI_AMSERVICE[] = "audiomanagement";
const char RSI_AM_SOURCERESOURCE[] = "sources";
const char RSI_AM_CONNECTIONRESOURCE[] = "connections";

const char RSI_AUDIOSTATUS_STRING[] = "audioStatus";
const char RSI_AUDIOSTATUS_STRING_MUTED[] = "muted";
const char RSI_AUDIOSTATUS_STRING_READYTOFADE[] = "readyToFade";
const char RSI_AUDIOSTATUS_STRING_FADEDIN[] = "fadedIn";

const char RSI_PLAYSTATUS_STRING[] = "playStatus";
const char RSI_PLAYSTATUS_STRING_IDLE[] = "idle";
const char RSI_PLAYSTATUS_STRING_PLAYING[] = "playing";

const char RSI_CONNECTIONSTATUS_STRING[] = "status";
const char RSI_CONNECTIONSTATUS_STRING_PREPARE[] = "prepare";
const char RSI_CONNECTIONSTATUS_STRING_STARTED[] = "started";
const char RSI_CONNECTIONSTATUS_STRING_PAUSED[] = "paused";
const char RSI_CONNECTIONSTATUS_STRING_STOPPED[] = "stopped";

const char RSI_CONNECTIONSOURCE_STRING[] = "source";

std::string createUUIDForSource(const std::string& sourceName)
{
   // GUID format as defined by JSON scheme: SHA1(name_of_service + '/' + name_of_resource + '/' + name_of_element)
   std::string stringRep(std::string(RSI_AMSERVICE) + "/" + std::string(RSI_AM_SOURCERESOURCE) + "/" + sourceName);

   tsd::common::checksums::SHA1 sha1Converter;
   tsd::common::types::ConstByteBuffer_t buffer(reinterpret_cast<const uint8_t*>(stringRep.c_str()), stringRep.size());
   sha1Converter.add(buffer);

   uint8_t shaBuffer[20];
   uint8_t* hashBuffer = sha1Converter.getHash(shaBuffer).get();

   std::stringstream temp;
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[0]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[1]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[2]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[3]);
   temp << "-";
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[4]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[5]);
   temp << "-";
   // UUID version (5) in fixed position (upper 4 bits)
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(((hashBuffer[6] & 0x0F) | 0x50));
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[7]);
   temp << "-";
   // UUID variant (always binary 10) in fixed position (upper 2 bits)
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(((hashBuffer[8] & 0x3F) | 0x80));
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[9]);
   temp << "-";
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[10]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[11]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[12]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[13]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[14]);
   temp << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(hashBuffer[15]);

   return temp.str();
}


RSISourceControl::RSISourceControl(
      const std::string& sourceName,
      ISourceListener* sourceListener,
      IAudioConnectionsListener* audioConnectionsListener,
      const std::string& ipAddress)
   : ISourceControl()
   , rsiclient::EventHandler()
   , m_Mutex()
   , m_SourceListener(sourceListener)
   , m_AudioConnectionsListener(audioConnectionsListener)
   , m_SourceName(sourceName)
   , m_SourceUUID(createUUIDForSource(sourceName))
   , m_Client(rsiclient::GetRsiClient(ipAddress, AUDIOMANAGEMENT_PORT))

   , m_SourceResourceSubscriptionId(INVALID_SUBSCRIPTION_ID)
   , m_SourceResourceSubscriptionUserId(11)

   , m_SourceElementSubscriptionId(INVALID_SUBSCRIPTION_ID)
   , m_SourceElementSubscriptionUserId(22)

   , m_ConnectionResourceSubscriptionId(INVALID_SUBSCRIPTION_ID)
   , m_ConnectionResourceSubscriptionUserId(33)

   , m_ConnectionElementSubscriptionUserId(44)

   , m_CurrentKnownConnections([](const Connection& a, const Connection& b){ return a.m_ConnectionName < b.m_ConnectionName; })
   , m_CurrentAudioStatus(AudioStatus_undefined)
   , m_CurrentPlayStatus(PlayStatus_undefined)
   , m_CurrentSourceAvailabilitiy(false)

   , m_WorkerThread("tsd.audio.sourcecontrol.worker")
{
   m_WorkerThread.start();

}

RSISourceControl::~RSISourceControl()
{
   m_Client->unsubscribe(m_SourceResourceSubscriptionId);
   m_Client->unsubscribe(m_SourceElementSubscriptionId);

   if (m_AudioConnectionsListener) {
      m_Client->unsubscribe(m_ConnectionResourceSubscriptionId);

      for (auto& connSubs : m_CurrentKnownConnections) {
         if (connSubs.m_SubscriptionId != INVALID_SUBSCRIPTION_ID) {
            m_Client->unsubscribe(connSubs.m_SubscriptionId);
         }
      }
   }
   m_WorkerThread.terminate();
}

bool
RSISourceControl::connect()
{
   m_Log.logInfo(__func__);
   // subscribe for resource
   auto subResource = subscribe(m_SourceResourceSubscriptionUserId, RSI_AM_SOURCERESOURCE);
   if (subResource != INVALID_SUBSCRIPTION_ID) {
      tsd::common::system::MutexGuard lock(m_Mutex);
      m_SourceResourceSubscriptionId = subResource;
   } else {
      m_Log.logWarn(__func__) << "source resource subscription failed" << std::endl;
      return false;
   }
   // subscribe for element
   auto subElement = subscribe(m_SourceElementSubscriptionUserId, RSI_AM_SOURCERESOURCE, m_SourceUUID);
   if (subElement != INVALID_SUBSCRIPTION_ID) {
      tsd::common::system::MutexGuard lock(m_Mutex);
      m_SourceElementSubscriptionId = subElement;
   } else {
      m_Log.logWarn(__func__) << "source element subscription failed" << std::endl;
      m_Client->unsubscribe(m_SourceResourceSubscriptionId); // cleanup
      return false;
   }

   // subscribe for the connections, if a listener was set
   if (m_AudioConnectionsListener) {
      // subscribe for resource
      auto subResource = subscribe(m_ConnectionResourceSubscriptionUserId, RSI_AM_CONNECTIONRESOURCE);
      if (subResource != INVALID_SUBSCRIPTION_ID) {
         tsd::common::system::MutexGuard lock(m_Mutex);
         m_ConnectionResourceSubscriptionId = subResource;
      } else {
         m_Log.logWarn(__func__) << "connection resource subscription failed" << std::endl;
         // TODO cleanup
         return false;;
      }
      // we try to get the resource and make sense of the content
      // if we don't get the resource we still connected successfully and are waiting for the subscriptions
      tsd::rsi::client::Response getResourceResponse;
      if (get(getResourceResponse, RSI_AM_CONNECTIONRESOURCE)) {
         if (auto object = dynamic_cast<rsidata::Object*>(getResourceResponse.getData())) {
            if (auto array = tsd::common::types::SmartPtr<rsidata::Array>(dynamic_cast<rsidata::Array*>(object->extract("data"))))
            {
               handleConnectionResourceData(array);
            }
            else
            {
               // no data element
               m_Log.logWarn(__func__) << "error: no data element" << std::endl;
            }
         }
      }
   }
   // get the source object, if available
   // if we don't get the element we still connected successfully and are waiting for the subscriptions
   tsd::rsi::client::Response getSourceElementResponse;
   if (get(getSourceElementResponse, RSI_AM_SOURCERESOURCE, m_SourceUUID)) {
      tsd::common::system::MutexGuard lock(m_Mutex);
      // we take the "data"
      if (auto object = dynamic_cast<rsidata::Object*>(getSourceElementResponse.getData())) {
         if (auto dataObject = tsd::common::types::SmartPtr<rsidata::Object>(dynamic_cast<rsidata::Object*>(object->extract("data"))))
         {
            handleSourceElementData(dataObject);
         } else {
            // no data or event element
            m_Log.logWarn(__func__) << "error: no data/event element" << std::endl;
         }
      }
   }
   // convenience: we send the source availability as an event on connect
   m_SourceListener->onSourceAvailabilityChanged(m_CurrentSourceAvailabilitiy);
   m_Log.logInfo(__func__) << __func__ << " done";

   return true;
}

bool
RSISourceControl::changePlayStatus(const PlayStatus newStatus)
{
   if ((m_SourceResourceSubscriptionId == INVALID_SUBSCRIPTION_ID) || (m_SourceElementSubscriptionId == INVALID_SUBSCRIPTION_ID)) {
      m_Log.logInfo(__func__) << "error: not connected" << std::endl;
      return false;
   }

   std::string statusString = [newStatus]()->std::string{
      switch (newStatus)
      {
         case (PlayStatus_idle): return RSI_PLAYSTATUS_STRING_IDLE;
         case (PlayStatus_playing): return RSI_PLAYSTATUS_STRING_PLAYING;
         default: return std::string();
      }
   }();
   if (statusString.empty()) {
      m_Log.logInfo(__func__) << "error: called with invalid playstatus " << newStatus << std::endl;
      return false;
   }

   auto dataObject = std::unique_ptr<rsidata::Object>(new rsidata::Object());
   dataObject->setNew(RSI_PLAYSTATUS_STRING, new rsidata::String(statusString));
   return post(std::move(dataObject), RSI_AM_SOURCERESOURCE, m_SourceUUID);
}

PlayStatus
RSISourceControl::getCurrentPlayStatus() const
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   return m_CurrentPlayStatus;
}

AudioStatus
RSISourceControl::getCurrentAudioStatus() const
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   return m_CurrentAudioStatus;
}

bool
RSISourceControl::getCurrentSourceAvailabilitiy() const
{
   tsd::common::system::MutexGuard lock(m_Mutex);
   return m_CurrentSourceAvailabilitiy;
}

bool
RSISourceControl::changeConnectionStatus(const std::string& connectionName, const ConnectionStatus newStatus)
{
   std::string statusString = [newStatus]()->std::string{
      switch (newStatus)
      {
         case (ConnectionStatus_started): return RSI_CONNECTIONSTATUS_STRING_STARTED;
         case (ConnectionStatus_stopped): return RSI_CONNECTIONSTATUS_STRING_STOPPED;
         default: return std::string();
      }
   }();
   if (statusString.empty()) {
      m_Log.logInfo(__func__) << "error: called with invalid connectionstatus " << newStatus << std::endl;
      return false;
   }

   for (auto& conn : m_CurrentKnownConnections) {
      if (conn.m_ConnectionName == connectionName) {
         auto dataObject = std::unique_ptr<rsidata::Object>(new rsidata::Object());
         dataObject->setNew(RSI_CONNECTIONSTATUS_STRING, new rsidata::String(statusString));
         return post(std::move(dataObject), RSI_AM_CONNECTIONRESOURCE, conn.m_ConnectionUUID);
      }
   }
   return false;
}

const std::vector<std::string> RSISourceControl::getCurrentAudioConnections() const
{
   return generateConnectionList();
}

void
RSISourceControl::handleEvent(
      const rsiclient::RequestState& state,
      const rsiclient::Response& response
   )
{
   if (auto object = dynamic_cast<rsidata::Object*>(response.getData())) {
      // status string only for responses
      if (auto statusString = std::unique_ptr<rsidata::String>(dynamic_cast<rsidata::String*>(object->extract("status")))) {
         if (statusString->get() != "ok") {
            // something went wrong
            m_Log.logWarn(__func__) << "error: status is " + statusString->get() << std::endl;
            return ;
         }
      }
      if (state) {
         if (response.getId() == m_SourceResourceSubscriptionUserId) {
            // we expect an array
            if (auto array = tsd::common::types::SmartPtr<rsidata::Array>(dynamic_cast<rsidata::Array*>(object->extract("data"))))
            {
               m_WorkerThread.push([this, array]{handleSourceResourceData(array);});
            }
            else
            {
               // no data element
               m_Log.logWarn(__func__) << "error: no data element" << std::endl;
            }
         }
         else if (response.getId() == m_SourceElementSubscriptionUserId)
         {
            // we take the "data"
            if (auto dataObject = tsd::common::types::SmartPtr<rsidata::Object>(dynamic_cast<rsidata::Object*>(object->extract("data"))))
            {
               m_WorkerThread.push([this, dataObject] {handleSourceElementData(dataObject);});
            } else {
               // no data or event element
               m_Log.logWarn(__func__) << "error: no data/event element" << std::endl;
            }

         }
         else if (response.getId() == m_ConnectionResourceSubscriptionUserId)
         {

            if (response.getId() == m_SourceResourceSubscriptionUserId) {
               // we expect an array
               if (auto array = tsd::common::types::SmartPtr<rsidata::Array>(dynamic_cast<rsidata::Array*>(object->extract("data"))))
               {
                  m_WorkerThread.push([this, array] {handleConnectionResourceData(array);});
               }
               else
               {
                  // no data element
                  m_Log.logWarn(__func__) << "error: no data element" << std::endl;
               }
            }
         }
         else if (response.getId() == m_ConnectionElementSubscriptionUserId)
         {
            // we take the "data"
            if (auto dataObject = tsd::common::types::SmartPtr<rsidata::Object>(dynamic_cast<rsidata::Object*>(object->extract("data"))))
            {
               m_WorkerThread.push([this, dataObject] {handleConnectionElementData(dataObject);});
            } else {
               // no data or event element
               m_Log.logWarn(__func__) << "error: no data/event element" << std::endl;
            }
         }
         else
         {
            m_Log.logInfo(__func__) << "RSISourceControl::handleEvent" << " received unknown event with id " << response.getId() << std::endl;
         }
      } else {
         m_Log.logInfo(__func__) << "RSISourceControl::handleEvent" <<  "received corrupt event" << std::endl;
      }
   }
}

bool
RSISourceControl::handleSourceElementData(tsd::common::types::SmartPtr<rsidata::Object> dataObject)
{
   auto errorFunction = [this](const std::string& logString) {
      m_Log.logWarn(__func__) << logString << std::endl;
      return false;
   };
   tsd::common::system::MutexGuard lock(m_Mutex);

   AudioStatus oldAS = m_CurrentAudioStatus;
   PlayStatus oldPS = m_CurrentPlayStatus;

   if (auto audioStatusString = std::unique_ptr<rsidata::String>(
          dynamic_cast<rsidata::String*>(dataObject->extract(RSI_AUDIOSTATUS_STRING)))) {
      if (audioStatusString->get() == RSI_AUDIOSTATUS_STRING_MUTED) {
         m_CurrentAudioStatus = AudioStatus_muted;
      } else if (audioStatusString->get() == RSI_AUDIOSTATUS_STRING_READYTOFADE) {
         m_CurrentAudioStatus = AudioStatus_readyToFade;
      } else if (audioStatusString->get() == RSI_AUDIOSTATUS_STRING_FADEDIN) {
         m_CurrentAudioStatus = AudioStatus_fadedIn;
      } else {
         // corrupt audioStatus
         return errorFunction(std::string("error: audiostatus unknown ") + audioStatusString->get());
      }
   } else {
      // no audioStatus
      return errorFunction("error: no audiostatus");
   }

   if (auto playStatusString = std::unique_ptr<rsidata::String>(
          dynamic_cast<rsidata::String*>(dataObject->extract(RSI_PLAYSTATUS_STRING)))) {
      if (playStatusString->get() == RSI_PLAYSTATUS_STRING_IDLE) {
         m_CurrentPlayStatus = PlayStatus_idle;
      } else if (playStatusString->get() == RSI_PLAYSTATUS_STRING_PLAYING) {
         m_CurrentPlayStatus = PlayStatus_playing;
      } else {
         // corrupt playStatus
         return errorFunction(std::string("error: playStatus unknown ") + playStatusString->get());
      }
   } else {
      // no playStatus
      return errorFunction("error: no playstatus");
   }

   if (oldAS != m_CurrentAudioStatus) {
      m_SourceListener->onAudioStatusChanged(m_CurrentAudioStatus);
   }

   if (oldPS != m_CurrentPlayStatus) {
      m_SourceListener->onPlayStatusChanged(m_CurrentPlayStatus);
   }
   return true;
}

void
RSISourceControl::handleSourceResourceData(tsd::common::types::SmartPtr<rsidata::Array> array)
{
   // evaluate RSI resource
   bool found = false;
   auto& arrayElements = array->get();
   for (auto e : arrayElements) {
      if (auto aObj = dynamic_cast<rsidata::Object*>(e)) {
         if (auto sourceName = std::unique_ptr<rsidata::String>(dynamic_cast<rsidata::String*>(aObj->extract("name")))) {
            if (sourceName->get() == m_SourceName) {
               found = true;
               break;
            }
         }
      }
   }

   tsd::common::system::MutexGuard lock(m_Mutex);
   if (m_CurrentSourceAvailabilitiy != found) {
      m_CurrentSourceAvailabilitiy = found;
      m_SourceListener->onSourceAvailabilityChanged(m_CurrentSourceAvailabilitiy);
   }
}

void
RSISourceControl::handleConnectionElementData(tsd::common::types::SmartPtr<rsidata::Object> connData)
{

   if (auto connName = std::unique_ptr<rsidata::String>(dynamic_cast<rsidata::String*>(connData->extract("name")))) {

      if (auto connStatus = std::unique_ptr<rsidata::String>(
             dynamic_cast<rsidata::String*>(connData->extract(RSI_CONNECTIONSTATUS_STRING)))) {

         if (connStatus->get() == RSI_CONNECTIONSTATUS_STRING_STARTED) {
            m_AudioConnectionsListener->onAudioConnectionStatusChanged(connName->get(), ConnectionStatus_started);
         } else if (connStatus->get() == RSI_CONNECTIONSTATUS_STRING_STOPPED) {
            m_AudioConnectionsListener->onAudioConnectionStatusChanged(connName->get(), ConnectionStatus_stopped);
         } else if (connStatus->get() == RSI_CONNECTIONSTATUS_STRING_PAUSED) {
            m_AudioConnectionsListener->onAudioConnectionStatusChanged(connName->get(), ConnectionStatus_paused);
         } else {
            m_Log.logInfo(__func__)
                     << "unhandled connection status " << connStatus->get() << " for conn " << connName->get() << std::endl;
         }
      } else {
         m_Log.logWarn(__func__) << "error: no status element" << std::endl;
      }
   } else {
      m_Log.logWarn(__func__) << "error: connection has no name" << std::endl;
   }
}

void
RSISourceControl::handleConnectionResourceData(tsd::common::types::SmartPtr<rsidata::Array> array)
{
   auto& arrayElements = array->get();
   for (auto e : arrayElements) {
      if (auto aObj = dynamic_cast<rsidata::Object*>(e)) {
         std::string connName;
         std::string connId;
         if (auto name = std::unique_ptr<rsidata::String>(dynamic_cast<rsidata::String*>(aObj->extract("name")))) {
            connName = name->get();
         }
         if (auto id = std::unique_ptr<rsidata::String>(dynamic_cast<rsidata::String*>(aObj->extract("id")))) {
            connId = id->get();
         }

         if ((!connName.empty()) && (!connId.empty())) {

            bool found = false;

            for (auto& conn : m_CurrentKnownConnections) {
               if (conn.m_ConnectionName == connName) {
                  assert(conn.m_ConnectionUUID == connId);
                  found = true;
                  break;
               }
            }

            if (!found) {
               // GET the connection
               rsiclient::Response getConnection;
               if (!get(getConnection, RSI_AM_CONNECTIONRESOURCE, connId) || (!getConnection.getData()) ) {
                  m_Log.logWarn(__func__) << "error: could not get connection " << connId << std::endl;
                  return;
               }

               if (auto connObj = dynamic_cast<rsidata::Object*>(getConnection.getData())) {
                  const auto& ptrs = connObj->get();
                  if (auto connData = dynamic_cast<rsidata::Object*>(ptrs["data"])) {
                     const auto& props = connData->get();
                     if (auto sourceObj = dynamic_cast<rsidata::Object*>(props[RSI_CONNECTIONSOURCE_STRING])) {

                        std::string sourceName;
                        std::string sourceId;

                        const auto& sourceProps = sourceObj->get();

                        if (auto sName = dynamic_cast<rsidata::String*>(sourceProps["name"])) {
                           sourceName = sName ->get();
                        }

                        if (auto sId = dynamic_cast<rsidata::String*>(sourceProps["id"])) {
                           sourceId = sId->get();
                        }

                        if (sourceName.empty() || sourceId.empty()) {
                           m_Log.logWarn(__func__)
                                    << "error: name: " << sourceName << ", id:" << sourceId << std::endl;
                           return;
                        }

                        Connection newConn;
                        newConn.m_ConnectionName = connName;
                        newConn.m_ConnectionUUID = connId;
                        newConn.m_SubscriptionId = INVALID_SUBSCRIPTION_ID;

                        bool informListener = false;
                        // check if we got a source "belonging" to this control object; if so, subscribe to it
                        if (sourceName == m_SourceName) {
                           auto subResult = subscribe(m_ConnectionElementSubscriptionUserId, RSI_AM_CONNECTIONRESOURCE, connId);
                           if (subResult != INVALID_SUBSCRIPTION_ID) {
                              newConn.m_SubscriptionId = subResult;
                           } else {
                              m_Log.logWarn(__func__)
                                       << "error: connection subscription failed" << std::endl;
                              return;
                           }
                           informListener = true;
                        }
                        tsd::common::system::MutexGuard lock(m_Mutex);
                        m_CurrentKnownConnections.insert(newConn);
                        if (informListener) {
                           m_AudioConnectionsListener->onAudioConnectionListChanged(generateConnectionList());
                           // TODO remove connection if vanished, for that we must not use the subscription ID as a visibility hint

                           if (auto object = dynamic_cast<rsidata::Object*>(getConnection.getData())) {

                              if (auto dataObject = tsd::common::types::SmartPtr<rsidata::Object>(dynamic_cast<rsidata::Object*>(object->extract("data"))))
                              {
                                 handleConnectionElementData(dataObject);
                              } else {
                                 // no data or event element
                                 m_Log.logWarn(__func__) << "error: no data/event element" << std::endl;
                              }
                           }
                        }

                     } else {
                        m_Log.logWarn(__func__) << "error: no source element" << std::endl;
                     }
                  } else {
                     m_Log.logWarn(__func__) << "error: connection has no data element" << std::endl;
                  }
               } else {
                  m_Log.logWarn(__func__) << "error: invalid data pointer" << std::endl;
               }
            }
         }
      }
   }
}

int32_t
RSISourceControl::subscribe(const int32_t subscriptionUserId, const std::string& resourceName, const std::string& elementUUID)
{
   rsiclient::Request subscribeRequest;
   subscribeRequest.m_Type = (elementUUID.empty())? rsiclient::SubscribeResource : rsiclient::SubscribeElement;
   subscribeRequest.m_Id = subscriptionUserId;
   subscribeRequest.m_Service = RSI_AMSERVICE;
   subscribeRequest.m_Resource = resourceName;
   subscribeRequest.m_Element = elementUUID;
   subscribeRequest.m_Query = std::map<std::string,std::string>();
   subscribeRequest.m_DataType = "";
   subscribeRequest.m_Data = std::unique_ptr<rsidata::Value>();
   subscribeRequest.m_UseBinaryData = false;
   subscribeRequest.m_BinaryData = std::vector<uint8_t>();
   subscribeRequest.m_DoEnDecoding = false;
   subscribeRequest.m_AcceptHeader = "";
   subscribeRequest.m_IfNoneMatchHeader = "";
   rsiclient::Response subscribeResponse;
   rsiclient::RequestState subscriptionState;
   int32_t subscrId = m_Client->subscribe(
            subscribeRequest,
            subscribeResponse,
            subscriptionState,
            *this
   );

   if (!subscriptionState) {
      return INVALID_SUBSCRIPTION_ID;
   } else {
      return subscrId;
   }
}

bool
RSISourceControl::get(rsi::client::Response& response, const std::string& resourceName, const std::string& elementUUID)
{
   std::map<std::string,std::string> queryParams;
   rsiclient::Request getRequest;
   getRequest.m_Type = (elementUUID.empty())? rsiclient::GetResource : rsiclient::GetElement;
   getRequest.m_Id = 0;
   getRequest.m_Service = RSI_AMSERVICE;
   getRequest.m_Resource = resourceName;
   getRequest.m_Element = elementUUID;
   getRequest.m_Query = queryParams;
   getRequest.m_DataType = "";
   getRequest.m_Data = std::unique_ptr<rsidata::Value>();
   getRequest.m_UseBinaryData = false;
   getRequest.m_BinaryData = std::vector<uint8_t>();
   getRequest.m_DoEnDecoding = false;
   getRequest.m_AcceptHeader = "";
   getRequest.m_IfNoneMatchHeader = "";
   return m_Client->sendRequest(getRequest, response);
}

bool
RSISourceControl::post(std::unique_ptr<rsidata::Value> dataToPost, const std::string& resourceName, const std::string& uuid)
{
   std::map<std::string,std::string> queryParams;
   rsiclient::Request postElementRequest;
   postElementRequest.m_Type = rsiclient::PostElement;
   postElementRequest.m_Id = 0;
   postElementRequest.m_Service = RSI_AMSERVICE;
   postElementRequest.m_Resource = resourceName;
   postElementRequest.m_Element = uuid;
   postElementRequest.m_Query = queryParams;
   postElementRequest.m_DataType = "application/JSON";
   postElementRequest.m_Data = std::unique_ptr<rsidata::Value>(std::move(dataToPost));
   postElementRequest.m_UseBinaryData = false;
   postElementRequest.m_BinaryData = std::vector<uint8_t>();
   postElementRequest.m_DoEnDecoding = false;
   postElementRequest.m_AcceptHeader = "";
   postElementRequest.m_IfNoneMatchHeader = "";
   rsiclient::Response postElementResponse;
   rsiclient::RequestState postElementRequestState = m_Client->sendRequest(postElementRequest, postElementResponse);

   if (!postElementRequestState) {
      m_Log.logInfo(__func__) << "error: post element to " << resourceName << " failed" << std::endl;
      return false;
   }
   return true;
}

const std::vector<std::string>
RSISourceControl::generateConnectionList() const
{
   std::vector<std::string> retData;
   for (auto& conn : m_CurrentKnownConnections) {
      if (conn.m_SubscriptionId != INVALID_SUBSCRIPTION_ID) {
         retData.push_back(conn.m_ConnectionName);
      }
   }
   return retData;
}

std::unique_ptr<ISourceControl>
createSourceControl(
   const std::string& sourceName,
   ISourceListener* sourceListener,
   IAudioConnectionsListener* connectionListener,
   const std::string& ipAddress
)
{
   return std::unique_ptr<ISourceControl>(new RSISourceControl(sourceName, sourceListener, connectionListener, ipAddress));
}

} // rsiapi
} // audio
} // app
} // android
} // sds
} // tsd

#endif
