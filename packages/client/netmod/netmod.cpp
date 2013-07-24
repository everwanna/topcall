#include "netmod.h"
#include "mutex.h"
#include "ioengine.h"
#include "connmgr.h"
#include "mempool.h"

#ifdef _WIN32
DWORD WINAPI _IOLoopWin(LPVOID)
{
	NET_LOG("_IOLoopWin, start.");
    IoEngine::Instance()->init();
    IoEngine::Instance()->run(NULL);

	NET_LOG("_IOLoopWin, start IoEngine::Release()");
	IoEngine::Release();		
	CConnMgr::Release();	
	MemPool::Release();
    AdaptLock::Release();

	NET_LOG("_IOLoopWin, exit.");

    return 0;
}
#else
void* _IOLoopUnix(void* ptr)
{
	NET_LOG("_IOLoopWin, start.");
    IoEngine::Instance()->init();
    IoEngine::Instance()->run(ptr);

	NET_LOG("_IOLoopWin, start IoEngine::Release()");
	IoEngine::Release();	
	CConnMgr::Release();
	MemPool::Release();
    AdaptLock::Release();

	NET_LOG("_IOLoopWin, exit.");

    return NULL;
}
#endif

namespace NetMod
{

#ifdef _WIN32
static HANDLE	s_hIOThread = NULL;
#endif

NETMOD_API int NetModStart()
{
	NET_LOG("NetModStart");

    int ret = 0;
    AdaptLock::Instance();
    CConnMgr::Instance();
    IoEngine::Instance();
    MemPool::Instance();
#ifdef _WIN32
    WSADATA wsaData;
    ret = ::WSAStartup( MAKEWORD(2,2), &wsaData );
    if (ret != NO_ERROR)
        return ret;

    s_hIOThread = CreateThread(NULL, 0, _IOLoopWin, NULL, 0, NULL);
    if (s_hIOThread == NULL) {
        return -1;
    }
    SetThreadPriority(s_hIOThread, THREAD_PRIORITY_ABOVE_NORMAL);
#else
    pthread_t id_IOLoop;
    ret = pthread_create(&id_IOLoop, NULL, &_IOLoopUnix, NULL);
    if ( 0 != ret )
        return ret;
//    pthread_join(id_IOLoop, NULL);
#endif
    return ret;
}

NETMOD_API int NetModStop()
{
	NET_LOG("NetModStop");

	if( s_hIOThread == NULL )
	{
		NET_LOG("NetModStop, s_hIOThread == NULL");
		return 0;
	}

	IoEngine::Instance()->stop();
#ifdef _WIN32
	//wait for the thread to stop here:	
	DWORD ret = WaitForSingleObject(s_hIOThread, 2000);
	switch(ret)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED: 		
	case WAIT_TIMEOUT:		
	case WAIT_FAILED:
		{
			NET_LOG("NetModStop, wait thread failed, error=", GetLastError());
			TerminateThread(s_hIOThread, -1);
		}		
		break;
	}
	CloseHandle(s_hIOThread);
	s_hIOThread = NULL;
#endif

    return 0;
}

NETMOD_API Packet* PacketAlloc(const char* data, size_t len)
{
    return MemPool::Instance()->newPacket(data, len);
}

NETMOD_API void PacketRelease(Packet* pkt)
{
    MemPool::Instance()->freePacket(pkt);
}

NETMOD_API int ConnCreate(ConnAttr* attr)
{
    return CConnMgr::Instance()->newConn(attr);
}

NETMOD_API int ConnConnect(int connid, uint32_t ip, uint16_t port)
{
    return CConnMgr::Instance()->connect(connid, ip, port);
}

NETMOD_API int ConnSend(int connid, Packet* pkt)
{
    return CConnMgr::Instance()->send(connid, pkt->_data, pkt->_dataLen);
}

NETMOD_API int ConnClose(int connid)
{
	//NET_LOG("ConnClose, connId=", connid);
	if( connid == 0 )
		return -1;

    CConnMgr::Instance()->close(connid);
    CConnMgr::Instance()->delayRemove(connid);
    return 0;
}

}
