//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/common/NullCheck.hpp
/// @brief Implementation of the functions in NullCheck.hpp
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_COMMON_NULLCHECK_HPP
#define TSD_SDS_ANDROID_COMMON_NULLCHECK_HPP

#include <tsd/sds/android/common/NullCheck.hpp>
#include <string>
#include <cxxabi.h>
#include <sstream>
#include <memory>

namespace tsd { namespace sds { namespace android { namespace common {

std::string getDemangledTypeName(const std::string &mangledTypeName, const std::string &variableName)
{
   std::stringstream typeName;
   int status(0);
   std::unique_ptr<char, void (*)(void*)> demangledTypeName{abi::__cxa_demangle(mangledTypeName.c_str(), nullptr, nullptr, &status), &std::free};
   if(0 == status)
   {
      const char* c = demangledTypeName.get();
      if(nullptr != c) do
      {
         switch(*c)
         {
         case ' ':
         case '>':
            break;
         case '<':
         case ',':
            typeName.put('.');
            break;
         case ':':
            typeName.put('.');
            if(*(++c) == '\0')
            {
               /* go back so that the while loop will end */
               --c;
            }
            break;
         default:
            typeName.put(*c);
         }
      } while(*(++c) != '\0');
   }
   else
   {
      typeName << mangledTypeName;

   }
   typeName << "::" << variableName;
   return typeName.str();
}

} // common
} // android
} // sds
} // tsd


#endif // TSD_SDS_ANDROID_COMMON_NULLCHECK_HPP
