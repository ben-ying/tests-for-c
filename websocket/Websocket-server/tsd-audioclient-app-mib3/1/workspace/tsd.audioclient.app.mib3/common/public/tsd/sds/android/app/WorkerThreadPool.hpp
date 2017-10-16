//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/WorkerThreadPool.hpp
/// @brief Definition of class WorkerThreadPool
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#ifndef TSD_SDS_ANDROID_APP_WORKERTHREADPOOL_HPP
#define TSD_SDS_ANDROID_APP_WORKERTHREADPOOL_HPP

#include <tsd/common/types/SmartPtr.hpp>
#include <tsd/common/system/Mutex.hpp>
#include <tsd/sds/utils/threads/IExceptionHandler.hpp>
#include <tsd/sds/utils/threads/SimpleWorkerThread.hpp>

namespace tsd { namespace sds { namespace android { namespace app {

/**
 * @brief Provides access to the threads (appart from the main-thread), used by the mib3-application.
 */
class WorkerThreadPool
{
public:
   /**
    * @brief The Constructor. Initializes the member variables.
    * @param[in] exceptionHandler The pointer to the class (object) implementing IExceptionHandler.
    */
   WorkerThreadPool(tsd::sds::utils::threads::IExceptionHandler* exceptionHandler);

   /**
    * @brief The Destructor.
    */
   virtual ~WorkerThreadPool();

   /**
    * @brief Used for short running tasks.
    * If one uses this instance for tasks, that take 'long' (e.g. language change or synthesis) the responsivity of the application may suffer.
    * @return a utils::threads::SimpleWorkerThread instance.
    */
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> getStateMachineWorkerThread();

   /**
    * @brief Returns the Thread responsible for the language change.
    * @return a utils::threads::SimpleWorkerThread instance.
    */
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> getLanguageChangeWorkerThread();

   /**
    * @brief Returns the Thread responsible for synthesis.
    * @return a utils::threads::SimpleWorkerThread instance.
    */
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> getCoreSynthesisWorkerThread();

   /**
    * @brief Returns the Thread responsible for playback.
    * @return a utils::threads::SimpleWorkerThread instance.
    */
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> getCorePlayerWorkerThread();

   /**
    * @brief Terminates all threads.
    * It is recommended to call this method before trying to shut down the application (otherwise the shutdown-behaviour may be eratic).
    */
   void cleanup();
private:
   tsd::sds::utils::threads::IExceptionHandler* m_ExceptionHandler;
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> m_StateMachineWorkerThread;
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> m_LanguageChangeWorkerThread;
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> m_CoreSynthesisWorkerThread;
   tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> m_CorePlayerWorkerThread;

   tsd::common::system::Mutex m_InstanceMutex;



};

} // app
} // android
} // sds
} // tsd


#endif // TSD_SDS_ANDROID_APP_WORKERTHREADPOOL_HPP
