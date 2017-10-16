//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/FileSystemAudioOut.cpp
/// @brief Implementation of class FileSystemAudioOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/FileSystemAudioOut.hpp"

#include <tsd/sds/utils/string/StringUtil.hpp>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

DEF_LOGGER(tsd::sds::android::app::audio::FileSystemAudioOut);

FileSystemAudioOut::FileSystemAudioOut(const std::string &fileName)
   : m_File(nullptr)
   , m_FileName(fileName)
{

}

FileSystemAudioOut::~FileSystemAudioOut()
{
   closeFile();
}

size_t FileSystemAudioOut::writeSamples(void *data, size_t len)
{
   m_Log.logInfo(__func__) << "len == " << len;
   if(nullptr != m_File)
   {
      return m_File->write((char*)data, len);
   }
   else
   {
      m_Log.logError(__func__) << "File instance is null.";
   }
   return 0;
}

void FileSystemAudioOut::finalize()
{
   m_Log.logInfo(__func__);
   closeFile();
}

void FileSystemAudioOut::start()
{
   bool success = false;
   m_File = new utils::io::File(m_FileName);
   if (m_File)
   {
      success = m_File->open(utils::io::File::WRITE_TRUNCATE);
   }
   if(!success)
   {
      m_Log.logError(__func__) << "Could not create file: " << m_File->absolutePath();
   }
}

bool FileSystemAudioOut::isStarted() const
{
   return m_File != nullptr && m_File->isOpen();
}

void FileSystemAudioOut::closeFile()
{
   if(m_File != nullptr)
   {
      m_File->close();
      delete m_File;
      m_File = nullptr;
   }
}


} // audio
} // app
} // android
} // sds
} // tsd

