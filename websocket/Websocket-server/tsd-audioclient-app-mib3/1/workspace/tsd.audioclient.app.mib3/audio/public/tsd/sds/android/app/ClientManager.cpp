/*
 * ClientManager.cpp
 *
 *  Created on: Oct 9, 2017
 *      Author: sunjianguang
 */
#include "ClientManager.hpp"
#include "tsd/sds/android/app/audio/AudioFactory.hpp"
#include <tsd/sds/android/app/config/AppConfigurationReader.hpp>
namespace tsd { namespace sds { namespace android { namespace app {
DEF_LOGGER(ClientManager);
ClientManager::ClientManager()
: m_AudioSession(nullptr)
, m_AudioFactory(nullptr)
, m_ConfigurationFile("config/android.json")//Greg :need to add this file
, m_UseAudioManagement(true)
, m_Configuration()
, m_WorkerThreadPool(this){
    config::AppConfigurationReader reader(m_Configuration);
    reader.read(m_ConfigurationFile);
}

ClientManager::~ClientManager(){

}

tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession>& ClientManager::getAudioSession()
{
   if (nullptr == m_AudioSession.get())
   {
      m_AudioSession = tsd::audio::audiorouter::client::ISession::create("androidAudio").release();
   }
   return m_AudioSession;
}

tsd::common::types::SmartPtr<audio::IAudioFactory> &ClientManager::getAudioFactory(){
    if(nullptr == m_AudioFactory.get())
    {
        m_AudioFactory = new audio::AudioFactory(getAudioSession(),m_WorkerThreadPool.getStateMachineWorkerThread(),  m_Configuration.getAudioConnectionMap(), m_UseAudioManagement);
    }
    return m_AudioFactory;
}

void ClientManager::handle(tsd::common::errors::BaseException &exception)
{
   m_Log.logError(__func__) << exception.getCause();
   //finish();
}

void ClientManager::handle(std::exception &exception)
{
   m_Log.logError(__func__) << exception.what();
   //finish();
}

void ClientManager::handle()
{
   m_Log.logError(__func__);
   //finish();
}

} // app
} // android
} // sds
} // tsd
