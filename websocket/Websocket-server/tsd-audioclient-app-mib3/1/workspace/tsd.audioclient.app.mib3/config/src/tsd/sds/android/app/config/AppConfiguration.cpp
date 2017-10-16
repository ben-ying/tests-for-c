//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/config/AppConfiguration.cpp
/// @brief Implementation of class AppConfiguration
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include <tsd/sds/android/app/config/AppConfiguration.hpp>
#include <tsd/sds/utils/config/GenericConfiguration.hpp>

namespace tsd { namespace sds { namespace android { namespace app { namespace config {

AppConfiguration::AppConfiguration()
   : m_CoreConfigurationRoot()
   , m_EngineConfigurationRoot()
   , m_CorePromptAudioBufferSize(0)
   , m_CorePromptStreamingReadySamples(0)
   , m_ToneIdMap()
   , m_AudioSourceMap()
   , m_AudioConnectionMap()
//   , m_VoicePropertyMap()
//   , m_PartitionList()
   , m_SourceToPartitionMap()
   , m_DefaultClientId()
   , m_DsiSynthesisClientId()
   , m_RsiSynthesisClientId()
   , m_DsiAudioBufferPartition()
   , m_MaximumAudioBufferSpace(0)
   , m_RsiServerPort(0)
   , m_IetfToNuanceMapRoot()
   , m_EppConfigDirectoryRoot()
{

}

AppConfiguration::~AppConfiguration()
{

}

void AppConfiguration::setCoreConfigurationRoot(const std::string &coreConfigurationRoot)
{
   m_CoreConfigurationRoot = coreConfigurationRoot;
}

const std::string &AppConfiguration::getCoreConfigurationRoot() const
{
   return m_CoreConfigurationRoot;
}

void AppConfiguration::setEngineConfigurationRoot(const std::string& engineConfigurationRoot)
{
   m_EngineConfigurationRoot = engineConfigurationRoot;
}

std::string &AppConfiguration::getEngineConfigurationRoot()
{
   return m_EngineConfigurationRoot;
}

const std::map<int32_t, std::string> &AppConfiguration::getToneIdMap() const
{
    return m_ToneIdMap;
}

void AppConfiguration::setToneIdMap(const std::map<int32_t, std::string> &toneIdMap)
{
    m_ToneIdMap = toneIdMap;
}

std::map<std::string, audio::AudioConnectionDescription>& AppConfiguration::getAudioConnectionMap()
{
    return m_AudioConnectionMap;
}

void AppConfiguration::setAudioConnectionMap(const std::map<std::string, audio::AudioConnectionDescription> &audioConnectionMap)
{
    m_AudioConnectionMap = audioConnectionMap;
}

std::map<std::string, std::string>& AppConfiguration::getAudioSourceMap()
{
   return m_AudioSourceMap;
}

void AppConfiguration::setAudioSourceMap(const std::map<std::string, std::string> &audioSourceMap)
{
   m_AudioSourceMap = audioSourceMap;
}

std::string AppConfiguration::getDsiSynthesisClientId() const
{
    return m_DsiSynthesisClientId;
}

void AppConfiguration::setDsiSynthesisClientId(const std::string &dsiSynthesisClientId)
{
    m_DsiSynthesisClientId = dsiSynthesisClientId;
}

std::string AppConfiguration::getRsiSynthesisClientId() const
{
   return m_RsiSynthesisClientId;
}

void AppConfiguration::setRsiSynthesisClientId(const std::string &RsiSynthesisClientId)
{
   m_RsiSynthesisClientId = RsiSynthesisClientId;
}

int16_t AppConfiguration::getRsiServerPort() const
{
   return m_RsiServerPort;
}

void AppConfiguration::setRsiServerPort(const int16_t &RsiServerPort)
{
   m_RsiServerPort = RsiServerPort;
}

/*void AppConfiguration::setVoicePropertyMap(const std::map<std::string, VoiceProperties> &voicePropertyMap)
{
   m_VoicePropertyMap = voicePropertyMap;
}

std::map<std::string, VoiceProperties> AppConfiguration::getVoicePropertyMap()
{
   return m_VoicePropertyMap;
}*/

int32_t AppConfiguration::getPersistenceLanguageNs() const
{
   return m_PersistenceLanguageNs;
}

void AppConfiguration::setPersistenceLanguageNs(const int32_t &persistenceLanguageNs)
{
   m_PersistenceLanguageNs = persistenceLanguageNs;
}

int32_t AppConfiguration::getPersistenceLanguageKey() const
{
   return m_PersistenceLanguageKey;
}

void AppConfiguration::setPersistenceLanguageKey(const int32_t &persistenceLanguageKey)
{
   m_PersistenceLanguageKey = persistenceLanguageKey;
}


void AppConfiguration::setIetfToNuanceMapRoot(const std::string &ietfToNuanceMapRoot)
{
   m_IetfToNuanceMapRoot = ietfToNuanceMapRoot;
}


std::string AppConfiguration::getIetfToNuanceMapRoot() const
{
   return m_IetfToNuanceMapRoot;
}


void AppConfiguration::setEppConfigDirectoryRoot(const std::string &eppConfigDirectoryRoot)
{
   m_EppConfigDirectoryRoot = eppConfigDirectoryRoot;
}

std::string AppConfiguration::getEppConfigDirectoryRoot() const
{
   return m_EppConfigDirectoryRoot;
}



void AppConfiguration::setMaximumAudioBufferSpace(const size_t& audioBufferSpace)
{
   m_MaximumAudioBufferSpace = audioBufferSpace;
}


size_t AppConfiguration::getMaximumAudioBufferSpace() const
{
   return m_MaximumAudioBufferSpace;
}


void AppConfiguration::setDefaultAudioBufferPartition(const std::string &partitionName)
{
   m_DefaultAudioBufferPartition = partitionName;
}


std::string AppConfiguration::getDefaultAudioBufferPartition() const
{
   return m_DefaultAudioBufferPartition;
}

void AppConfiguration::setDsiAudioBufferPartition(const std::string &partitionName)
{
   m_DsiAudioBufferPartition = partitionName;
}


std::string AppConfiguration::getDsiAudioBufferPartition() const
{
   return m_DsiAudioBufferPartition;
}


/*void AppConfiguration::setAudioMemoryPoolPartitions(const tsd::sds::android::app::memory::PartitionList& partitionList)
{
   m_PartitionList = partitionList;
}


tsd::sds::android::app::memory::PartitionList AppConfiguration::getAudioMemoryPoolPartitions() const
{
   return m_PartitionList;
}*/


void AppConfiguration::setRsiSourceToAudioMemoryPartitionMap(std::map<std::string, std::string>& sourceToPartitionMap)
{
   m_SourceToPartitionMap = sourceToPartitionMap;
}


std::map<std::string, std::string> AppConfiguration::getRsiSourceToAudioMemoryPartitionMap() const
{
   return m_SourceToPartitionMap;
}


} // config
} // app
} // android
} // sds
} // tsd

