#ifndef BIONIC_AUTO_LOCK_DEF
#define BIONIC_AUTO_LOCK_DEF

//-----------------------------------------------------------
// helpers for thread synchronization
//-----------------------------------------------------------
#if defined(WIN32)

//-----------------------------------------------------------------------------------------
// A busy-wait lock to synchronize the access to shared data between user mode and kernel mode code.
//-----------------------------------------------------------------------------------------
typedef long volatile BUSY_LOCK;
__inline void BusyLockInitialize(BUSY_LOCK &theLock) 
{
   theLock = 0;
}
__inline void BusyLockAcquire(BUSY_LOCK &theLock) 
{
   while (InterlockedCompareExchange((LONG*)&theLock, -1, 0) != 0);
}
__inline void BusyLockRelease(BUSY_LOCK &theLock)
{
   InterlockedExchange((LONG *)&theLock, 0);
}

#endif

#ifdef __cplusplus

template<class Lockable>
class AutoLock
{
public:
   AutoLock(Lockable & lock) : m_lock(lock)
   {
      lock.Lock();
   }
   ~AutoLock()
   {
      m_lock.Unlock();
   }
private:
   Lockable & m_lock;
};

//
// Helper class for using a CRITICAL_SECTION SYNC object with automatically 
// unlocking function to simplify the usage of a critical section
// and can avoid the mis-match of Enter/Leave critical section.
// Usage:
// {   <----- '{}' scope
//     AutoLock lock(&xxx_cs)
//     ..... // the code need be protected.
// }   <-----
#ifdef WIN32
class MutexLock
{
public:
   MutexLock()
   {
      InitializeCriticalSection(&m_lock);
   }
   ~MutexLock()
   {
      DeleteCriticalSection(&m_lock);
   }
   void Lock() 
   {
      EnterCriticalSection(&m_lock);
   }
   void Unlock() 
   {
      LeaveCriticalSection(&m_lock);
   }
private:
   CRITICAL_SECTION m_lock;
};

template<>
class AutoLock<CRITICAL_SECTION>
{
public:
   AutoLock(CRITICAL_SECTION & lock) : m_lock(lock)
   {
      EnterCriticalSection(&m_lock);
   }
   ~AutoLock()
   {
      LeaveCriticalSection(&m_lock);
   }
private:
   CRITICAL_SECTION & m_lock;
};
typedef AutoLock<CRITICAL_SECTION> CsAutoLock;

template<>
class AutoLock<BUSY_LOCK>
{
public:
   AutoLock(BUSY_LOCK & lock) : m_lock(lock)
   {
      BusyLockAcquire(m_lock);
   }
   ~AutoLock()
   {
      BusyLockRelease(m_lock);
   }
private:
   BUSY_LOCK & m_lock;
};
typedef AutoLock<BUSY_LOCK>  BusyAutoLock;

#endif

#endif

#endif