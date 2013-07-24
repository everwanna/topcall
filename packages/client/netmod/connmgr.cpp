#include "connmgr.h"

CConnMgr* CConnMgr::m_pInstance = NULL;

CConnMgr::CConnMgr()
{
}

CConnMgr::~CConnMgr()
{
	removeAll();
}

CConnMgr* CConnMgr::Instance()
{
    if ( NULL == m_pInstance )
    {   //no locker here coz NetModStart will call this function to make sure m_pInstance is inited first!!!
        m_pInstance = new CConnMgr();
        //LinkLayerEnc::genRSAKey();
    }
    return m_pInstance;
}

void CConnMgr::Release()
{
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

int CConnMgr::newConn(ConnAttr* attr)
{
    CConn * pConn = new CConn();
    int connid = pConn->init(attr);
    if (connid == -1)
    {
        delete pConn;
        return -1;
    }
    AdaptLock::Instance()->lock();
    m_connMap.insert(std::make_pair(connid, pConn));
    AdaptLock::Instance()->unlock();

    return connid;
}

void CConnMgr::removeConn(int connid)
{
    AdaptLock::Instance()->lock();
    connmap_t::iterator iFind = m_connMap.find(connid);
    if ( iFind != m_connMap.end() )
    {
		NET_LOG("CConnMgr::removeConn, connId=", connid);
        delete iFind->second;        
        m_connMap.erase(iFind);
    }
	else
	{
		NET_LOG("CConnMgr::removeConn, conn not found, connid=", connid);
	}
    AdaptLock::Instance()->unlock();
}

void CConnMgr::removeAll()
{
	NET_LOG("CConnMgr::removeAll");
    AdaptLock::Instance()->lock();
    for ( connmap_t::iterator io = m_connMap.begin(); io != m_connMap.end(); ++io )
    {
        if (io->second)
            delete io->second;
    }
    m_connMap.clear();
	m_delayConnSet.clear();
    AdaptLock::Instance()->unlock();
}

int CConnMgr::connect(int connid, uint32_t ip, uint16_t port)
{
	int ret = -1;
    AdaptLock::Instance()->lock();
    connmap_t::const_iterator iFind = m_connMap.find(connid);
    if ( iFind != m_connMap.end() )
        ret = iFind->second->connect(ip, port);
    AdaptLock::Instance()->unlock();

    return ret;
}

int CConnMgr::send(int connid, char* data, size_t len)
{
	int ret = -1;
    AdaptLock::Instance()->lock();
    connmap_t::const_iterator iFind = m_connMap.find(connid);
    if ( iFind != m_connMap.end() )
        ret = iFind->second->send(data, len);
    AdaptLock::Instance()->unlock();

    return ret;
}

int CConnMgr::close(int connid)
{
	int ret = -1;
    AdaptLock::Instance()->lock();
    connmap_t::const_iterator iFind = m_connMap.find(connid);
    if ( iFind != m_connMap.end() )
        ret = iFind->second->close();
    AdaptLock::Instance()->unlock();

    return ret;
}

void	CConnMgr::delayRemove(int connid)
{
    AdaptLock::Instance()->lock();
	m_delayConnSet.insert(connid);
    AdaptLock::Instance()->unlock();
}

void	CConnMgr::checkDelayRemove()
{
	if( m_delayConnSet.empty() )
		return;

    AdaptLock::Instance()->lock();
	for( connset_t::const_iterator it = m_delayConnSet.begin(); it != m_delayConnSet.end(); ++it )
	{		
		NET_LOG("CConnMgr::checkDelayRemove, connid=", *it);
		removeConn(*it);
	}
	m_delayConnSet.clear();
    AdaptLock::Instance()->unlock();
}

/******************************************
int CConnMgr::onRecv(SOCKET s)
{
    AutoLock autoLock(m_lock_map);
    std::map<SOCKET, CConn*>::const_iterator iFind = m_connMap.find(s);
    if ( iFind != m_connMap.end() )
        return iFind->second->onRecv();

    return -1;
}

int CConnMgr::onSend(SOCKET s)
{
    AutoLock autoLock(m_lock_map);
    std::map<SOCKET, CConn*>::const_iterator iFind = m_connMap.find(s);
    if ( iFind != m_connMap.end() )
        return iFind->second->onSend();

    return -1;
}

*/