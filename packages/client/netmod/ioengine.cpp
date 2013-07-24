#include "ioengine.h"
#include "conn.h"
#include "mempool.h"
#include "connmgr.h"
#include "mutex.h"

IoEngine* IoEngine::m_pInstance = NULL;

IoEngine::IoEngine()
: m_nStopping(0)
, m_uMaxFd(0)
, m_uDelayCheck(0)
{
    m_readSoSet.clear();
    m_writeSoSet.clear();
    m_connMap.clear();
}

IoEngine::~IoEngine()
{
}

bool IoEngine::init()
{
    m_nStopping = 0;	
    return true;
}

IoEngine* IoEngine::Instance()
{
    if ( NULL == m_pInstance ) //no need locker here coz NetModStart will call this function to make sure m_pInstance is inited first!!!
    {
        m_pInstance = new IoEngine();
    }
    return m_pInstance;
}

void	IoEngine::Release()
{
	NET_LOG("IoEngine::Release enter.");
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	NET_LOG("IoEngine::Release exit.");
}

void IoEngine::stop()
{
	AdaptLock::Instance()->lock();
    ATOMIC_ADD(&m_nStopping);
	AdaptLock::Instance()->unlock();
}

void IoEngine::setEvent(IConn* pConn, SOCKET s, uint16_t opt, bool bAdd)
{
    AdaptLock::Instance()->lock();
    if ( bAdd )
    {
        if ( opt & FD_OP_READ )
            m_readSoSet.insert(s);
        if ( opt & FD_OP_WRITE )
            m_writeSoSet.insert(s);

        m_connMap.insert(std::make_pair(s, pConn));
    }
    else
    {
        if ( opt & FD_OP_READ && !m_readSoSet.empty() )
            m_readSoSet.erase(s);
        if ( opt & FD_OP_WRITE && !m_writeSoSet.empty() )
            m_writeSoSet.erase(s);
    }
    
    if ( opt & FD_OP_CLR )
    {
		if( !m_readSoSet.empty() )
			m_readSoSet.erase(s);
		if( !m_writeSoSet.empty() )
			m_writeSoSet.erase(s);

		m_connMap.erase(s);
    }
    AdaptLock::Instance()->unlock();
}

void IoEngine::run(void*)
{
    fd_set  FdSetRead;
    fd_set  FdSetWrite;

    struct timeval mtimeout;
    while (!m_nStopping)
    {
        m_uMaxFd = 0;
        FD_ZERO(&FdSetRead);
        FD_ZERO(&FdSetWrite);

        mtimeout.tv_sec = 0;
        mtimeout.tv_usec = 500; //500ms

		if( m_readSoSet.empty() && m_writeSoSet.empty() )
		{
			::Sleep(20);
			continue;
		}
		//NET_LOG("IoEngine::run, read/write/connMap=", m_readSoSet.size(), m_writeSoSet.size(), m_connMap.size());

        std::set<SOCKET> ReadSoSet;	// = m_readSoSet;
        std::set<SOCKET> WriteSoSet;// = m_writeSoSet;

        AdaptLock::Instance()->lock();
		ReadSoSet.insert(m_readSoSet.begin(), m_readSoSet.end());
		WriteSoSet.insert(m_writeSoSet.begin(), m_writeSoSet.end());
        AdaptLock::Instance()->unlock();

        for (std::set<SOCKET>::const_iterator io = ReadSoSet.begin(); io != ReadSoSet.end(); ++io)
        {
            FD_SET(*io, &FdSetRead);
            m_uMaxFd = m_uMaxFd < *io ? *io : m_uMaxFd;
        }
        for (std::set<SOCKET>::const_iterator io = WriteSoSet.begin(); io != WriteSoSet.end(); ++io)
        {
            FD_SET(*io, &FdSetWrite);
            m_uMaxFd = m_uMaxFd < *io ? *io : m_uMaxFd;
        }

        //in WIN32, the first param in select() is ignored, but in unix(linux),it's important and it's value is the max of readfd,writefd,expfd +1
        int ret = ::select( m_uMaxFd+1 , &FdSetRead, &FdSetWrite, NULL, &mtimeout );
        if ( 0 >= ret ) //error
		{
			::Sleep(20);
            continue;
		}

        //no listen socket fd because it's client process, not server
        for (std::set<SOCKET>::const_iterator io = ReadSoSet.begin(); io != ReadSoSet.end(); ++io)
        {
            SOCKET so = *io;
            if ( FD_ISSET(so , &FdSetRead) )
            {
                _onRecv(so);
            }
        }
        for (std::set<SOCKET>::const_iterator io = WriteSoSet.begin(); io != WriteSoSet.end(); ++io)
        {
            SOCKET so = *io;
            if ( FD_ISSET(so, &FdSetWrite) )
            {
                _onSend(so);
            }
        }
		
		m_uDelayCheck++;
		if( m_uDelayCheck >= 30 )
		{
			CConnMgr::Instance()->checkDelayRemove();
			m_uDelayCheck = 0;
		}		
    }
	NET_LOG("IoEngine::run, exit.");
}

void IoEngine::_onRecv(SOCKET s)
{
    //NET_LOG("IoEngine::_onRecv, socket=",s);
    /*************************************************************
    * conn objects can be deleted only by netmod thread!!! so when u
    * find conn that u want, release the lock to make sure it won't
    * cause dead lock, and the conn won't be deleted in this callback
    **************************************************************/
    IConn* pConn = NULL;
    AdaptLock::Instance()->lock();
    std::map<SOCKET, IConn*>::const_iterator io = m_connMap.find(s);
    if ( io != m_connMap.end() )
        pConn = io->second;
    AdaptLock::Instance()->unlock();
    if ( pConn )
        pConn->onRecv();
}

void IoEngine::_onSend(SOCKET s)
{
    //NET_LOG("IoEngine::_onSend, socket=",s);
    /*************************************************************
    * conn objects can be deleted only by netmod thread!!! so when u
    * find conn that u want, release the lock to make sure it won't
    * cause dead lock, and the conn won't be deleted in this callback
    **************************************************************/
    IConn* pConn = NULL;
    AdaptLock::Instance()->lock();
    std::map<SOCKET, IConn*>::const_iterator io = m_connMap.find(s);
    if (io != m_connMap.end())
        pConn = io->second;
    AdaptLock::Instance()->unlock();
    if ( pConn )
        pConn->onSend();
}