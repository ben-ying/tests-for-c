//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/config/AppConfiguration.hpp
/// @brief Definition of class AppConfiguration
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATION_HPP
#define TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATION_HPP

#include <string>
#include <tsd/audio/audiorouter/client/ISession.hpp>
#include <tsd/sds/android/app/audio/AudioConnectionDescription.hpp>
//#include <tsd/sds/android/app/memory/PartitionDescription.hpp>
#include <tsd/sds/utils/config/GenericConfiguration.hpp>
//#include <tsd/sds/android/app/config/VoiceProperties.hpp>
#include <map>

namespace tsd { namespace sds { namespace android { namespace app { namespace config {

/**
 * @brief Provides access to static configuration values needed by various components of the application.
 */
class AppConfiguration
{
public:
   AppConfiguration();
   virtual ~AppConfiguration();

   /**
    * @brief Sets the 'root' for the configuration of tsd.sds.android.core.
    * The String does not need to represent a directory in the fileSystem. The value depends on the implementation of tsd.sds.android.ICore.
    * @param[in] coreConfigurationRoot the new core configuration root.
    */
   virtual void setCoreConfigurationRoot(const std::string& coreConfigurationRoot);

   /**
    * @brief Returns the core configuration root.
    * The returned value does not need to represent a directory in the fileSystem (as the name suggests), it's rather an abstract value,
    * that has to contain the information how the used tsd.sds.android.core.ICore instance shall be configures.
    */
   virtual const std::string &getCoreConfigurationRoot() const;

   /**
    * @brief Sets the root-directory for the engine-configuration. Needed to configure the module tsd.sds.android.engine.ve.
    * @param[in] engineConfigurationRoot
    */
   virtual void setEngineConfigurationRoot(const std::string &engineConfigurationRoot);

   /**
    * @brief Returns the root-directory for the engine-configuration. Needed to configure the module tsd.sds.android.engine.ve
    */
   virtual std::string& getEngineConfigurationRoot();

   /**
    * @brief Returns the mapping of tone ids to the corresponding path to the tone file in the file system.
    */
   const std::map<int32_t, std::string>& getToneIdMap() const;

   /**
    * @brief Sets the mapping of tone ids to the corresponding path to the tone file in the file system.
    * @param[in] toneIdMap
    */
   void setToneIdMap(const std::map<int32_t, std::string> &toneIdMap);

   /**
    * @brief Returns the mapping from connection ID to audio channel.
    * The audio channel is used to create audio-out instances in the audio router.
    */
   std::map<std::string, audio::AudioConnectionDescription> &getAudioConnectionMap();

   /**
    * @brief Sets the mapping from connection ID to audio channel.
    * @param[in] audioConnectionMap new audio connection map
    */
   void setAudioConnectionMap(const std::map<std::string, audio::AudioConnectionDescription> &audioConnectionMap);

   /**
    * @brief Returns the mapping from source ID to audio channel.
    * The audio channel is used to create audio-out instances in the audio router.
    */
   std::map<std::string, std::string> &getAudioSourceMap();

   /**
    * @brief Sets the mapping from source ID to audio channel.
    * @param[in] audioSourceMap new audio source map
    */
   void setAudioSourceMap(const std::map<std::string, std::string> &audioSourceMap);

   /**
    * @brief Returns the audio session.
    */
   std::shared_ptr<tsd::audio::audiorouter::client::ISession>& getAudioSession() const;

   /**
    * @brief Sets audio session.
    * @param[in] audioSession new audio session
    */
   void setAudioSession(std::shared_ptr<tsd::audio::audiorouter::client::ISession>& audioSession);

   /**
    * @brief Returns the synthesis client id for the DsiAdapter
    */
   std::string getDsiSynthesisClientId() const;

   /**
    * @brief Sets the synthesis client id for the DsiAdapter
    * @param[in] dsiSynthesisClientId
    */
   void setDsiSynthesisClientId(const std::string &dsiSynthesisClientId);

   /**
    * @brief Returns the synthesis client id for the RsiAdapter
    */
   std::string getRsiSynthesisClientId() const;

   /**
    * @brief Sets the synthesis client id for the RsiAdapter
    * @param[in] rsiSynthesisClientId
    */
   void setRsiSynthesisClientId(const std::string &RsiSynthesisClientId);

   /**
    * @brief Returns the rsi server port.
    */
   int16_t getRsiServerPort() const;

   /**
    * @brief Sets the rsi server port.
    * @param[in] RsiServerPort
    */
   void setRsiServerPort(const int16_t &RsiServerPort);

/*   *
    * @brief Sets the voice configuration map.
    * @param[in] voicePropertyMap the voice configuration map.

   void setVoicePropertyMap(const std::map<std::string, config::VoiceProperties>& voicePropertyMap);

   *
    * @brief Returns the voice property map.

   std::map<std::string, config::VoiceProperties> getVoicePropertyMap();*/

   /**
    * @brief Returns the namespace id for the persistence client, used to load/store the language/voice value.
    */
   int32_t getPersistenceLanguageNs() const;
   /**
    * @brief Sets the namespace id for the persistence client, used to load/store the language/voice value.
    * @param[in] persistenceLanguageNs the namespace id
    */
   void setPersistenceLanguageNs(const int32_t &persistenceLanguageNs);

   /**
    * @brief Returns the key for the persistence client, used to load/store the language/voice value.
    */
   int32_t getPersistenceLanguageKey() const;
   /**
    * @brief Sets the key for the persistence client, used to load/store the language/voice value.
    * @param[in] persistenceLanguageKey the key
    */
   void setPersistenceLanguageKey(const int32_t &persistenceLanguageKey);

   /** @brief Sets the path to the Ietf-to-Naunce mapping file.
    * @param[in] ietfToNuanceMapRoot The path to the Ietf-to-Nuance mapping file.
    */
   void setIetfToNuanceMapRoot(const std::string& ietfToNuanceMapRoot);

   /**
    * @brief Returns the path to the Ietf-Nuance mapping file.
    */
   std::string getIetfToNuanceMapRoot() const;

   /**
    * @brief Sets the root to the directory containing language specific sub-directories for EPP config.
    * @param[in] eppConfigDirectoryRoot The path to the directory containing language specific sub-directories for EPP config.
    */
   void setEppConfigDirectoryRoot(const std::string& eppConfigDirectoryRoot);

   /**
    * @brief Returns the path to the directory containing language specific sub-directories for EPP config.
    */
   std::string getEppConfigDirectoryRoot() const;

   /** @brief Sets the maximum amount of memory useable be used for audio buffers.
    *  @param[in] audioBufferSpace Maximum buffer space in bytes.
    */
   void setMaximumAudioBufferSpace(const size_t& audioBufferSpace);

   /**
    * @brief Returns maximum audio buffer space in bytes.
    */
   size_t getMaximumAudioBufferSpace() const;

   /**
    * @brief Sets the default audio buffer partition name. This name is used if there is no valid partition name for an audio buffer.
    * @param[in] partitionName Partition name
    */
   void setDefaultAudioBufferPartition(const std::string &partitionName);

   /**
    * @brief Returns the name of the default audio buffer partition
    */
   std::string getDefaultAudioBufferPartition() const;

   /**
    * @brief Sets the DSI audio buffer partition name
    * @param[in] partitionName Partition name
    */
   void setDsiAudioBufferPartition(const std::string &partitionName);

   /**
    * @brief Returns the name of the DSI audio buffer partition
    */
   std::string getDsiAudioBufferPartition() const;

/*   *
    * @brief Sets the list of memory pool partitions for audio buffers
    * @param[in] partitionList List of partition descriptions

   void setAudioMemoryPoolPartitions(const tsd::sds::android::app::memory::PartitionList& partitionList);

   *
    * @brief Returns the list of memory pool partitions for audio buffers

   tsd::sds::android::app::memory::PartitionList getAudioMemoryPoolPartitions() const;*/

   /**
    * @brief Sets the mapping from source ID to memory pool partitions for audio buffers
    * @param[in] sourceToPartitionMap Maps source IDs to memory pool partitions for audio buffers
    */
   void setRsiSourceToAudioMemoryPartitionMap(std::map<std::string, std::string>& sourceToPartitionMap);

   /**
    * @brief Returns the mapping from source ID to memory pool partitions for audio buffers
    */
   std::map<std::string, std::string> getRsiSourceToAudioMemoryPartitionMap() const;

private:

   std::string m_CoreConfigurationRoot;
   std::string m_EngineConfigurationRoot;
   size_t m_CorePromptAudioBufferSize;
   size_t m_CorePromptStreamingReadySamples;
   std::map<int32_t, std::string> m_ToneIdMap;
   std::map<std::string, std::string> m_AudioSourceMap;
   std::map<std::string, audio::AudioConnectionDescription> m_AudioConnectionMap;
//   std::map<std::string, config::VoiceProperties> m_VoicePropertyMap;
//   tsd::sds::android::app::memory::PartitionList m_PartitionList;
   std::map<std::string, std::string> m_SourceToPartitionMap;
   std::string m_DefaultClientId;
   std::string m_DsiSynthesisClientId;
   std::string m_RsiSynthesisClientId;
   int32_t m_PersistenceLanguageNs;
   int32_t m_PersistenceLanguageKey;
   std::string m_DefaultAudioBufferPartition;
   std::string m_DsiAudioBufferPartition;
   size_t m_MaximumAudioBufferSpace;
   int16_t m_RsiServerPort;
   std::string m_IetfToNuanceMapRoot;
   std::string m_EppConfigDirectoryRoot;
};

} // config
} // app
} // android
} // sds
} // tsd


#endif // TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATION_HPP
