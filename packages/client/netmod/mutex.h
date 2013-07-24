#ifndef _NETMOD_MUTEX_LOCK_H_
#define _NETMOD_MUTEX_LOCK_H_
#include "netmod.h"

//actually it's a wrapper for multi-platforms
class MutexLock
{
public:
    MutexLock(wchar_t* name);
    ~MutexLock();

public:
    void lock();
    void unlock();

private:
#ifdef _WIN32
    HANDLE m_lk;
    //CRITICAL_SECTION	m_lk;
#else
    pthread_mutex_t&  m_lk;
#endif
};

class AdaptLock
{
public:
    AdaptLock();
    ~AdaptLock();

public:
    void lock();
    void unlock();

    static AdaptLock* Instance();
    static void Release();

private:
    static AdaptLock*   m_pIns;
    MutexLock*  m_pLock;
};


//---------------------------------------------
class AutoLock
{
public:
    AutoLock(MutexLock* lock);
    ~AutoLock();

private:
    MutexLock*  m_lk;
};

//----------------------------------------

#endif


