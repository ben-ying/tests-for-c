//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/FileSystemAudioOut.hpp
/// @brief Definition of class FileSystemAudioOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_AUDIO_FILESYSTEMAUDIOOUT_HPP
#define TSD_SDS_ANDROID_APP_AUDIO_FILESYSTEMAUDIOOUT_HPP


#include <tsd/common/types/typedef.hpp>
#include <tsd/sds/android/core/backend/playback/IAudioOut.hpp>
#include <tsd/sds/utils/logging/Logger.hpp>
#include <tsd/sds/utils/io/File.hpp>
#include <string>


namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

/**
 * @brief Implementation of @see core::prompt::playback::IAudioOut
 * Writes samples to a file given in the fileName in the constructor.
 */
class FileSystemAudioOut : public tsd::sds::android::core::backend::playback::IAudioOut
{
public:

   /**
    * @brief FileSystemAudioOut Constructor.
    * @param[in] fileName the name of the file, samples shall be written to.
    */
   FileSystemAudioOut(const std::string& fileName);

   /**
    * @brief ~FileSystemAudioOut Destructor.
    */
   virtual ~FileSystemAudioOut();

   /**
    * @brief @see core::prompt::playback::IAudioOut::writeSamples()
     */
   virtual size_t writeSamples(void* data, size_t len) override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::finalize()
    */
   virtual void finalize() override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::start()
    */
   virtual void start() override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::istStarted()
    */
   virtual bool isStarted() const override;

   /**
    * @brief @see core::prompt::playback::IAudioOut::initialize()
    */
   virtual void initialize() override {}

private:

   void closeFile();

   utils::io::File* m_File;
   std::string m_FileName;
   DECL_LOGGER();
};


} // audio
} // app
} // android
} // sds
} // tsd

#endif // TSD_SDS_TTS_APP_AUDIO_FILESYSTEMAUDIOOUT_HPP
