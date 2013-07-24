#include "mutex.h"

AdaptLock* AdaptLock::m_pIns = NULL; //for m_connMap in ConnMgr and IoEngine(both 2 resources)

AdaptLock::AdaptLock()
{
    m_pLock = new MutexLock(L"Netmod_AdaptLock_");
}

AdaptLock::~AdaptLock()
{
    delete m_pLock;
}

AdaptLock* AdaptLock::Instance()
{
    if ( !m_pIns )
    {
        m_pIns = new AdaptLock();
    }
    return m_pIns;
}

void AdaptLock::Release()
{
    if ( m_pIns )
    {
        delete m_pIns;
        m_pIns = NULL;
    }
}

void AdaptLock::lock()
{
    m_pLock->lock();
}

void AdaptLock::unlock()
{
    m_pLock->unlock();
}

//--------------------------------
MutexLock::MutexLock(wchar_t* name)
{
#ifdef _WIN32
    std::wstring mutexName = std::wstring(name);
    wchar_t buf[20];
    swprintf(buf, 20, L"%u", ::GetCurrentProcessId());
    mutexName += std::wstring(buf);
    swprintf(buf, 20, L"_%u", ::GetTickCount());
    mutexName += std::wstring(buf);
    m_lk = ::CreateMutex(NULL, FALSE, mutexName.c_str());
    //InitializeCriticalSection(&m_lk);
#else
    pthread_mutex_init(&m_lk, NULL);
#endif
}

MutexLock::~MutexLock()
{
#ifdef _WIN32
    ::CloseHandle(m_lk);
    //DeleteCriticalSection(&m_lk);
#else
    pthread_mutex_destroy(&m_lk);
#endif
}

void MutexLock::lock()
{
#ifdef _WIN32
    //EnterCriticalSection(&m_lk);

    switch( ::WaitForSingleObject(m_lk, INFINITE) )
    {
    case WAIT_OBJECT_0:
        break;
    case WAIT_ABANDONED: 
        break; 
    }
#else
    pthread_mutex_lock(&m_lk);
#endif
}

void MutexLock::unlock()
{
#ifdef _WIN32
    //LeaveCriticalSection(&m_lk);
    ReleaseMutex(m_lk);
#else
    pthread_mutex_unlock(&m_lk);
#endif
}

//-----------------------------------
AutoLock::AutoLock(MutexLock* lock)
: m_lk(lock)
{
    m_lk->lock();
}

AutoLock::~AutoLock()
{
    m_lk->unlock();
}



