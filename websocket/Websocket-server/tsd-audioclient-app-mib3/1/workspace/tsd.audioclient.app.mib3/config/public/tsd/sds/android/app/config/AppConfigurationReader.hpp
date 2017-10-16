//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/config/AppConfigurationReader.hpp
/// @brief Definition of class AppConfigurationReader
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATIONREADER_HPP
#define TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATIONREADER_HPP

#include <string>
#include <tsd/sds/android/app/config/AppConfiguration.hpp>
#include <tsd/sds/utils/config/GenericConfiguration.hpp>
#include <tsd/dataformats/data/visitor.hpp>

namespace tsd { namespace sds { namespace android { namespace app { namespace config {

/**
 * @brief Reads a json document and writes the results in an instance of AppConfiguration
 */
class AppConfigurationReader : public dataformats::data::Visitor
{
public:
   /**
    * @brief Constructor
    * @param[in] appConfiguration destination for read configuration values.
    */
   AppConfigurationReader(AppConfiguration& appConfiguration);
   virtual ~AppConfigurationReader();

   /**
    * @brief Reads the json document in @p jsonFile to the AppConfiguration instance given in the constructor
    * @param[in] jsonFile contains the json document. Source for configuration values.
    */
   void read(const std::string& jsonFile);
private:

   // inherited from dataformats::Visitor.
   virtual void visitNull() override;

   virtual void visitBoolean(bool) override;
   virtual void visitInteger(int32_t) override;

   virtual void visitFloat(double value) override;

   virtual void visitString(dataformats::ConstByteBuffer_t data) override;

   virtual void visitBlob(dataformats::ConstByteBuffer_t) override;

   virtual void visitArray(tsd::common::types::PtrVector<dataformats::data::Value>&data) override;

   virtual void visitObject(tsd::common::types::PtrMap<std::string, dataformats::data::Value>& data) override;

   AppConfiguration& m_Configuration;
   std::string m_CurrentKey;

   DECL_LOGGER();



};


} // config
} // app
} // android
} // sds
} // tsd


#endif // TSD_SDS_ANDROID_APP_CONFIG_APPCONFIGURATIONREADER_HPP
