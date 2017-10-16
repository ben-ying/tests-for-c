//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/common/NullCheck.hpp
/// @brief Contains functions to check variables for null and throw an exception.
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_AUDIO_COMMON_NULLCHECK_HPP
#define TSD_SDS_AUDIO_COMMON_NULLCHECK_HPP

#include <tsd/common/errors/InvalidArgumentException.hpp>
#include <memory>

namespace tsd { namespace sds { namespace android { namespace common {

/// Checks the variable in @p Variable for NULL. If the variable is null, an tsd::common::errors::InvalidArgumentException is thrown.
/// If thrown, the exception contains the information about the class where the variable is located is part of the exception.
/// Usage: ANDROID_THROW_IF_NULL(m_CoreAdapter, RsiPlayer);
#define ANDROID_THROW_IF_NULL(Variable, Class) tsd::sds::android::common::throwIfNull((Variable), (#Variable), typeid(Class).name())

/**
 * @brief Builds a 'human readable' name from @p mangledTypeName (the result of typeid(className).name() and @p variableName
 * @param[in] mangledTypeName the mangled type name of a class
 * @param[in] variableName the name of a member of the class.
 * @return the demangled 'human readable' fully qualified name of the member variable @p variableName of the class given in @p mangledTypeName.
 */
std::string getDemangledTypeName(const std::string& mangledTypeName, const std::string& variableName);

/**
 * @brief Checks the given argument for null and throws an InvalidArgumentException if  arg is null.
 * @param[in] reason  reason used to create the exception.
 */
template<typename T> static void throwIfNullPrivate(T arg, const std::string& reason)
{
   if(nullptr == arg)
   {
      throw tsd::common::errors::InvalidArgumentException(reason,true);
   }
}

/**
 * @brief Checks the given argument for null and throws an InvalidArgumentException if  arg is null.
 * @param[in] arg the argument, that shall be checked for null.
 * @param[in] variableName the name of the variable, used to build the reason for the exception.
 * @param[in] className the name of the class, the variable is a member of, used to build the reason for the exception.
 */

template<typename T> void throwIfNull(T* arg, const std::string& variableName, const std::string& className)
{
   const std::string demangledTypeName = getDemangledTypeName(className, variableName) ;
   throwIfNullPrivate(arg, demangledTypeName);
}

/**
 * @brief Checks the given argument for null and throws an InvalidArgumentException if  arg.get() is null.
 * @param[in] arg the argument, that shall be checked for null.
 * @param[in] variableName the name of the variable, used to build the reason for the exception.
 * @param[in] className the name of the class, the variable is a member of, used to build the reason for the exception.
 */

template<typename T> void throwIfNull(tsd::common::types::SmartPtr<T>& arg, const std::string& variableName, const std::string& className)
{
   const std::string demangledTypeName = getDemangledTypeName(className, variableName) ;
   throwIfNullPrivate(arg.get(), demangledTypeName);
}

/**
 * @brief Checks the given argument for null and throws an InvalidArgumentException if  arg.get() is null.
 * @param[in] arg the argument, that shall be checked for null.
 * @param[in] variableName the name of the variable, used to build the reason for the exception.
 * @param[in] className the name of the class, the variable is a member of, used to build the reason for the exception.
 */
template<typename T> void throwIfNull(std::unique_ptr<T>& arg, const std::string& variableName, const std::string& className)
{
   const std::string demangledTypeName = getDemangledTypeName(className, variableName);
   throwIfNullPrivate(arg.get(), demangledTypeName);
}

} // common
} // android
} // sds
} // tsd


#endif // TSD_SDS_ANDROID_COMMON_NULLCHECK_HPP
