#ifndef _NET_MODULE_CONNECTION_MANAGER_H_
#define _NET_MODULE_CONNECTION_MANAGER_H_
#include "conn.h"
#include "mutex.h"
#include <map>
#include <set>

class CConnMgr
{
public:
    CConnMgr();
    ~CConnMgr();

public:
    static CConnMgr* Instance();
	static void Release();

public:
    int     newConn(ConnAttr* attr);
    void    removeConn(int connid);
    void    removeAll();
    //int     UnregEvHandler(IEventHandler* ev);

public:
    int     connect(int connid, uint32_t ip, uint16_t port);
    int     send(int connid, char* data, size_t len);
    int     close(int connid);
	void	delayRemove(int connid);
	void	checkDelayRemove();

    /*************************
public:
    int     onRecv(SOCKET s);
    int     onSend(SOCKET s);
    ****************************/

private:
    typedef std::map<int, CConn*> connmap_t;
	typedef std::set<int> connset_t;
    connmap_t           m_connMap;
	connset_t			m_delayConnSet;

    static CConnMgr*    m_pInstance;
};

#endif


