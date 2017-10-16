//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/WorkerThreadPool.cpp
/// @brief Implementation of class WorkerThreadPool
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include <tsd/sds/android/app/WorkerThreadPool.hpp>
#include <tsd/common/system/MutexGuard.hpp>

namespace tsd { namespace sds { namespace android { namespace app {

WorkerThreadPool::WorkerThreadPool(utils::threads::IExceptionHandler *exceptionHandler)
   : m_ExceptionHandler(exceptionHandler)
   , m_StateMachineWorkerThread(nullptr)
   , m_LanguageChangeWorkerThread(nullptr)
   , m_CoreSynthesisWorkerThread(nullptr)
   , m_CorePlayerWorkerThread(nullptr)
   , m_InstanceMutex()
{

}

WorkerThreadPool::~WorkerThreadPool()
{

}

tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> WorkerThreadPool::getStateMachineWorkerThread()
{
   tsd::common::system::MutexGuard lock(m_InstanceMutex);
   if(nullptr == m_StateMachineWorkerThread.get())
   {
      m_StateMachineWorkerThread = new utils::threads::SimpleWorkerThread("tsd.sds.android.app.audioWorkerThread", m_ExceptionHandler);
      m_StateMachineWorkerThread->start();
   }
   return m_StateMachineWorkerThread;
}

tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> WorkerThreadPool::getLanguageChangeWorkerThread()
{
   tsd::common::system::MutexGuard lock(m_InstanceMutex);
   if(nullptr == m_LanguageChangeWorkerThread.get())
   {
      m_LanguageChangeWorkerThread = new utils::threads::SimpleWorkerThread("tsd.sds.android.app.languageChangeWorkerThread", m_ExceptionHandler);
      m_LanguageChangeWorkerThread->start();
   }
   return m_LanguageChangeWorkerThread;
}

tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> WorkerThreadPool::getCoreSynthesisWorkerThread()
{
   tsd::common::system::MutexGuard lock(m_InstanceMutex);
   if(nullptr == m_CoreSynthesisWorkerThread.get())
   {
      m_CoreSynthesisWorkerThread = new utils::threads::SimpleWorkerThread("tsd.sds.android.app.coreSynthesisWorkerThread", m_ExceptionHandler);
      m_CoreSynthesisWorkerThread->start();
   }
   return m_CoreSynthesisWorkerThread;
}

tsd::common::types::SmartPtr<utils::threads::SimpleWorkerThread> WorkerThreadPool::getCorePlayerWorkerThread()
{
   tsd::common::system::MutexGuard lock(m_InstanceMutex);
   if(nullptr == m_CorePlayerWorkerThread.get())
   {
      m_CorePlayerWorkerThread = new utils::threads::SimpleWorkerThread("tsd.sds.android.app.corePlayerWorkerThread", m_ExceptionHandler);
      m_CorePlayerWorkerThread->start();
   }
   return m_CorePlayerWorkerThread;
}

void WorkerThreadPool::cleanup()
{
   tsd::common::system::MutexGuard lock(m_InstanceMutex);
   if(nullptr != m_StateMachineWorkerThread.get()) m_StateMachineWorkerThread->terminate();
   if(nullptr != m_CorePlayerWorkerThread.get()) m_CorePlayerWorkerThread->terminate();
   if(nullptr != m_CoreSynthesisWorkerThread.get()) m_CoreSynthesisWorkerThread->terminate();
   if(nullptr != m_LanguageChangeWorkerThread.get()) m_LanguageChangeWorkerThread->terminate();
}

} // app
} // android
} // sds
} // tsd

