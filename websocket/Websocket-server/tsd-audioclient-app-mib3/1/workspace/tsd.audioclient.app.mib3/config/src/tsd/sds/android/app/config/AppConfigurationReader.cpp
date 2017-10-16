//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/config/AppConfigurationReader.cpp
/// @brief Implementation of class AppConfigurationReader
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include <tsd/sds/android/app/config/AppConfigurationReader.hpp>
//#include <tsd/sds/android/app/config/VoiceConfigurationReader.hpp>
#include <tsd/dataformats/json/utils.hpp>
#include <cmath>

namespace tsd { namespace sds { namespace android { namespace app { namespace config {

DEF_LOGGER(AppConfigurationReader);

class ToneMappingVisitor : public dataformats::data::Visitor
{
public:
   ToneMappingVisitor(int32_t& toneId, std::string& path);
   virtual ~ToneMappingVisitor();

   // inherited from dataformats::Visitor.
   virtual void visitNull(){}
   virtual void visitBoolean(bool){}
   virtual void visitInteger(int32_t value);
   virtual void visitFloat(double){}
   virtual void visitString(dataformats::ConstByteBuffer_t data);
   virtual void visitBlob(dataformats::ConstByteBuffer_t){}
   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&){}
   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data);

private:
   int32_t& m_ToneId;
   std::string& m_Path;

   std::string m_CurrentKey;

};


/**
 * @brief Helper used to parse entries in the source mapping array of the configuration
 */
class SourceMappingVisitor : public dataformats::data::Visitor
{
public:
   SourceMappingVisitor(std::string& sourceId, std::string& channelId);
   virtual ~SourceMappingVisitor();

   // inherited from dataformats::Visitor.
   virtual void visitNull() override{}
   virtual void visitBoolean(bool) override {}
   virtual void visitInteger(int32_t) override{}
   virtual void visitFloat(double) override{}
   virtual void visitString(dataformats::ConstByteBuffer_t data) override;
   virtual void visitBlob(dataformats::ConstByteBuffer_t) override{}
   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&/*data*/) override{}
   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data) override;
private:
   std::string& m_SourceId;
   std::string& m_AudioChannelId;
   std::string m_CurrentKey;
};


/**
 * @brief Helper used to parse entries in the connection mapping array of the configuration
 */
class ConnectionMappingVisitor : public dataformats::data::Visitor
{
public:
   ConnectionMappingVisitor(std::string& connectionId, audio::AudioConnectionDescription &connectionDescription);
   virtual ~ConnectionMappingVisitor();

   // inherited from dataformats::Visitor.
   virtual void visitNull() override{}
   virtual void visitBoolean(bool) override {}
   virtual void visitInteger(int32_t) override{}
   virtual void visitFloat(double) override{}
   virtual void visitString(dataformats::ConstByteBuffer_t data) override;
   virtual void visitBlob(dataformats::ConstByteBuffer_t) override{}
   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&/*data*/) override{}
   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data) override;
private:
   std::string& m_ConnectionId;
   audio::AudioConnectionDescription& m_AudioConnectionDescription;
   std::string m_CurrentKey;
};

/**
 * @brief Helper used to parse entries in the audio memory partition list of the configuration
 */
/*class PartitionListVisitor : public dataformats::data::Visitor
{
public:
   PartitionListVisitor(tsd::sds::android::app::memory::PartitionDescription& partitionDescription);
   virtual ~PartitionListVisitor();

   // inherited from dataformats::Visitor.

   virtual void visitNull() override{}
   virtual void visitBoolean(bool) override {}
   virtual void visitInteger(int32_t) override;
   virtual void visitFloat(double) override{}
   virtual void visitString(dataformats::ConstByteBuffer_t data) override;
   virtual void visitBlob(dataformats::ConstByteBuffer_t) override{}
   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&data) override{}
   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data) override;
private:
   tsd::sds::android::app::memory::PartitionDescription& m_PartitionDescription;
   std::string m_CurrentKey;
};*/

/**
 * @brief Helper used to parse entries in the RSI-source-to-audio-memory-partition map of the configuration
 */
class SourceToPartitionMapVisitor : public dataformats::data::Visitor
{
public:
   SourceToPartitionMapVisitor(std::string& sourceName, std::string& partitionName);
   virtual ~SourceToPartitionMapVisitor();

   // inherited from dataformats::Visitor.

   virtual void visitNull() override{}
   virtual void visitBoolean(bool) override {}
   virtual void visitInteger(int32_t) override {}
   virtual void visitFloat(double) override{}
   virtual void visitString(dataformats::ConstByteBuffer_t data) override;
   virtual void visitBlob(dataformats::ConstByteBuffer_t) override{}
   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&/*data*/) override{}
   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data) override;
private:
   std::string& m_SourceName;
   std::string& m_PartitionName;
   std::string m_CurrentKey;
};


AppConfigurationReader::AppConfigurationReader(AppConfiguration &appConfiguration)
   : dataformats::data::Visitor()
   , m_Configuration(appConfiguration)
   , m_CurrentKey()
{

}

AppConfigurationReader::~AppConfigurationReader()
{

}

void AppConfigurationReader::read(const std::string &jsonFile)
{
   std::unique_ptr<dataformats::data::Value> value = dataformats::json::valueFromJsonFile(jsonFile);
   visit(value.get());
}

void AppConfigurationReader::visitNull()
{

}

void AppConfigurationReader::visitBoolean(bool /*value*/)
{

}

void AppConfigurationReader::visitInteger(int32_t value)
{
   m_Log.logInfo(__func__) << "Found value: " << std::hex << value;
   if("tsd.sds.android.app.mib3.rsi.serverPort" == m_CurrentKey)
   {
      m_Configuration.setRsiServerPort(static_cast<int16_t>(value));
   }
   else if("tsd.sds.android.app.mib3.persistence.language.ns" == m_CurrentKey)
   {
      m_Configuration.setPersistenceLanguageNs(value);
   }
   else if("tsd.sds.android.app.mib3.persistence.language.key" == m_CurrentKey)
   {
      m_Configuration.setPersistenceLanguageKey(value);
   }
   else if ("tsd.sds.android.app.mib3.maximumAudioBufferSpaceInKb" == m_CurrentKey)
   {
      if (value > 0)
      {
         m_Configuration.setMaximumAudioBufferSpace(static_cast<size_t>(value) * 1000); // kB to byte
      }
   }
}

void AppConfigurationReader::visitFloat(double value)
{
   if("tsd.sds.android.app.mib3.persistence.language.ns" == m_CurrentKey)
   {
      // because the value is too big for a simple integer, we have to cast the float to
      // a uint first (otherwise, we loose some information).
      const uint32_t uintValue = static_cast<uint32_t>(value);
      m_Configuration.setPersistenceLanguageNs(static_cast<int32_t>(uintValue));
   }
   else if ("tsd.sds.android.app.mib3.maximumAudioBufferSpaceInKb" == m_CurrentKey)
   {
      if (value > 0.0)
      {
         m_Configuration.setMaximumAudioBufferSpace(static_cast<size_t>(std::lround(value+0.5)) * 1000); // kB to byte
      }
   }
}

void AppConfigurationReader::visitString(dataformats::ConstByteBuffer_t data)
{
   if("tsd.sds.android.app.mib3.coreConfigRoot" == m_CurrentKey)
   {
      m_Configuration.setCoreConfigurationRoot(data.bufferData());
   }
   else if("tsd.sds.android.app.mib3.engineConfigRoot" == m_CurrentKey)
   {
      m_Configuration.setEngineConfigurationRoot(data.bufferData());
   }
   else if("tsd.sds.android.app.mib3.voiceConfigDirectory" == m_CurrentKey)
   {
/*      std::map<std::string, VoiceProperties> voiceConfigMap;
      config::VoiceConfigurationReader reader(data.bufferData());
      reader.read(voiceConfigMap);
      m_Configuration.setVoicePropertyMap(voiceConfigMap);*/
   }
   else if("tsd.sds.android.app.mib3.ietfToNuanceMapRoot" == m_CurrentKey)
   {
      m_Configuration.setIetfToNuanceMapRoot(data.bufferData());
   }
   else if("tsd.sds.android.app.mib3.eppConfigDirectoryRoot" == m_CurrentKey)
   {
      m_Configuration.setEppConfigDirectoryRoot(data.bufferData());
   }
   else if("tsd.sds.android.app.mib3.dsiSynthesisClientId" == m_CurrentKey)
   {
      m_Configuration.setDsiSynthesisClientId(data.bufferData());
   }
   else if("tsd.sds.android.app.mib3.rsiSynthesisClientId" == m_CurrentKey)
   {
      m_Configuration.setRsiSynthesisClientId(data.bufferData());
   }
   else if ("tsd.sds.android.app.mib3.defaultAudioBufferPartition" == m_CurrentKey)
   {
      m_Configuration.setDefaultAudioBufferPartition(data.bufferData());
   }
   else if ("tsd.sds.android.app.mib3.dsiAudioBufferPartition" == m_CurrentKey)
   {
      m_Configuration.setDsiAudioBufferPartition(data.bufferData());
   }
}

void AppConfigurationReader::visitBlob(dataformats::ConstByteBuffer_t /*data*/)
{

}

void AppConfigurationReader::visitArray(tsd::common::types::PtrVector<dataformats::data::Value> &data)
{
   if("tsd.sds.android.app.mib3.toneIdMap" == m_CurrentKey)
   {
      std::map<int32_t, std::string> toneMapping;
      for(auto it : data)
      {
         int32_t toneId = 0xaffe;
         std::string path;
         ToneMappingVisitor visitor(toneId, path);
         visitor.visit(it);
         if(!path.empty() && toneId != 0xaffe)
         {
            toneMapping.insert(std::make_pair(toneId, path));
            m_Log.logInfo(__func__) << "found tone mapping: " << toneId << " --> '" << path << "'";
         }
         else
         {
            m_Log.logError(__func__) << "not a valid mapping: " << toneId << " --> '" << path << "'";
         }
      }
      m_Configuration.setToneIdMap(toneMapping);
   }
   else if("tsd.sds.android.app.mib3.audioSourceMapping" == m_CurrentKey)
   {

      std::map<std::string, std::string> sourceMapping;
      for(const auto& value : data)
      {
         std::string sourceId;
         std::string channelId;

         SourceMappingVisitor mappingVisitor(sourceId, channelId);
         mappingVisitor.visit(value);
         if(!sourceId.empty() && !channelId.empty())
         {
            sourceMapping.insert(std::make_pair(sourceId, channelId));
            m_Log.logInfo(__func__) << " channel mapping found: '" << sourceId
                                    << "' --> '" <<  channelId << "'";
         }
         else
         {
            m_Log.logError(__func__) << "not a valid channel mapping: '" << sourceId
                                     << "' --> '" <<  channelId << "'";
         }
         m_Configuration.setAudioSourceMap(sourceMapping);
      }

   }
   else if("tsd.sds.android.app.mib3.audioConnectionMapping" == m_CurrentKey)
   {
      std::map<std::string, audio::AudioConnectionDescription> connectionMapping;
      for(const auto& value : data)
      {
         std::string connectionId;
         audio::AudioConnectionDescription connectionDescription;

         ConnectionMappingVisitor mappingVisitor(connectionId, connectionDescription);
         mappingVisitor.visit(value);
         if(!connectionId.empty() && !connectionDescription.m_AudioChannelId.empty())
         {
            connectionMapping.insert(std::make_pair(connectionId, connectionDescription));
            m_Log.logInfo(__func__) << " connection mapping found: '" << connectionId
                                    << "' --> '" <<  connectionDescription.m_AudioChannelId << "'";
         }
         else
         {
            m_Log.logError(__func__) << "not a valid connection mapping: '" << connectionId
                                     << "' --> '" <<  connectionDescription.m_AudioChannelId << "'";
         }
         m_Configuration.setAudioConnectionMap(connectionMapping);
      }
   }
   else if("tsd.sds.android.app.mib3.audioMemoryPoolPartitions" == m_CurrentKey)
   {
      // parse and store the partition list
/*      tsd::sds::android::app::memory::PartitionList partitionList;
      partitionList.reserve(data.size());

      for(const auto& value : data)
      {
         tsd::sds::android::app::memory::PartitionDescription partitionDescription;

         PartitionListVisitor mappingVisitor(partitionDescription);
         mappingVisitor.visit(value);
         if(!partitionDescription.m_Id.empty() && partitionDescription.m_Priority > 0)
         {
            partitionList.push_back(partitionDescription);
            m_Log.logInfo(__func__) << " partition found with name '" << partitionDescription.m_Id
                                    << "', PMB size (in bytes)" <<  partitionDescription.m_ProtectedMemoryBlockSize
                                    << " and priority " << partitionDescription.m_Priority;
         }
         else
         {
            m_Log.logError(__func__) << "invalid partition description found";
            // @todo: What to do if a partition description is invalid? The whole pool might not be usable
         }
         m_Configuration.setAudioMemoryPoolPartitions(partitionList);
      }*/
   }
   else if("tsd.sds.android.app.mib3.rsiSourceToAudioMemoryPoolPartitionMap" == m_CurrentKey)
   {
      // parse and store the source-to-partition map
      std::map<std::string, std::string> sourceToPartitionMapping;
      for(const auto& value : data)
      {
         std::string source;
         std::string partitionName;

         SourceToPartitionMapVisitor mappingVisitor(source, partitionName);
         mappingVisitor.visit(value);
         if(!source.empty() && !partitionName.empty())
         {
            sourceToPartitionMapping.insert(std::make_pair(source, partitionName));
            m_Log.logInfo(__func__) << " source mapping found: '" << source
                                    << "' --> '" << partitionName << "'";
         }
         else
         {
            m_Log.logError(__func__) << "not a valid source mapping: '" << source
                                     << "' --> '" << partitionName << "'";
         }
         m_Configuration.setRsiSourceToAudioMemoryPartitionMap(sourceToPartitionMapping);
      }
   }
}

void AppConfigurationReader::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      m_Log.logInfo(__func__) << "Current Key: " << m_CurrentKey;
      visit(element.second);
   }
}

/****************************************
 * Implementation of ToneMappingVisitor *
 ****************************************/
ToneMappingVisitor::ToneMappingVisitor(int32_t &toneId, std::string &path)
   : dataformats::data::Visitor()
   , m_ToneId(toneId)
   , m_Path(path)
   , m_CurrentKey()
{

}

ToneMappingVisitor::~ToneMappingVisitor()
{

}

void ToneMappingVisitor::visitInteger(int32_t value)
{
   if("toneId" == m_CurrentKey)
   {
      m_ToneId = value;
   }
}

void ToneMappingVisitor::visitString(dataformats::ConstByteBuffer_t data)
{
   if("path" == m_CurrentKey)
   {
      m_Path = data.bufferData();
   }
}

void ToneMappingVisitor::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      visit(element.second);
   }
}

/**********************************************
 * Implementation of SourceMappingVisitor *
 **********************************************/
SourceMappingVisitor::SourceMappingVisitor(std::string& sourceId, std::string& channelId)
   : m_SourceId(sourceId)
   , m_AudioChannelId(channelId)
   , m_CurrentKey()
{

}

SourceMappingVisitor::~SourceMappingVisitor()
{

}

void SourceMappingVisitor::visitString(dataformats::ConstByteBuffer_t data)
{
   if("sourceId" == m_CurrentKey)
   {
      m_SourceId = data.bufferData();
   }
   else if("channelId" == m_CurrentKey)
   {
      m_AudioChannelId = data.bufferData();
   }
}

void SourceMappingVisitor::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      visit(element.second);
   }
}
/**********************************************
 * Implementation of ConnectionMappingVisitor *
 **********************************************/
ConnectionMappingVisitor::ConnectionMappingVisitor(std::string &connectionId, audio::AudioConnectionDescription &connectionDescription)
   : m_ConnectionId(connectionId)
   , m_AudioConnectionDescription(connectionDescription)
   , m_CurrentKey()
{

}

ConnectionMappingVisitor::~ConnectionMappingVisitor()
{

}

void ConnectionMappingVisitor::visitString(dataformats::ConstByteBuffer_t data)
{
   if("connectionId" == m_CurrentKey)
   {
      m_ConnectionId = data.bufferData();
   }
   else if("connectionUuid" == m_CurrentKey)
   {
      m_AudioConnectionDescription.m_AudioConnectionUuId = data.bufferData();
   }
   else if("channelId" == m_CurrentKey)
   {
      m_AudioConnectionDescription.m_AudioChannelId = data.bufferData();
   }
}

void ConnectionMappingVisitor::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      visit(element.second);
   }
}

/******************************************
 * Implementation of PartitionListVisitor *
 ******************************************/
/*
PartitionListVisitor::PartitionListVisitor(tsd::sds::android::app::memory::PartitionDescription& partitionDescription)
   : m_PartitionDescription(partitionDescription)
   , m_CurrentKey()
{
}

PartitionListVisitor::~PartitionListVisitor()
{
}

void PartitionListVisitor::visitString(dataformats::ConstByteBuffer_t data)
{
   if("partitionName" == m_CurrentKey)
   {
      m_PartitionDescription.m_Id = data.bufferData();
   }
}

void PartitionListVisitor::visitInteger(int32_t value)
{
   if("priority" == m_CurrentKey)
   {
      m_PartitionDescription.m_Priority = std::abs(value);
   }
   else if("protectedMemoryBlockSizeInKb" == m_CurrentKey)
   {
      if (value >= 0)
      {
         m_PartitionDescription.m_ProtectedMemoryBlockSize = static_cast<size_t>(value) * 1000; // kB to byte
      }
   }

}

void PartitionListVisitor::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      visit(element.second);
   }
}
*/

/*************************************************
 * Implementation of SourceToPartitionMapVisitor *
 *************************************************/
SourceToPartitionMapVisitor::SourceToPartitionMapVisitor(std::string& sourceName, std::string& partitionName)
   : m_SourceName(sourceName)
   , m_PartitionName(partitionName)
   , m_CurrentKey()
{
}

SourceToPartitionMapVisitor::~SourceToPartitionMapVisitor()
{
}

void SourceToPartitionMapVisitor::visitString(dataformats::ConstByteBuffer_t data)
{
   if("source" == m_CurrentKey)
   {
      m_SourceName = data.bufferData();
   }
   else if("partitionName" == m_CurrentKey)
   {
      m_PartitionName = data.bufferData();
   }
}

void SourceToPartitionMapVisitor::visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value> &data)
{
   for(const auto& element : data)
   {
      m_CurrentKey = element.first;
      visit(element.second);
   }
}

} // config
} // app
} // android
} // sds
} // tsd

