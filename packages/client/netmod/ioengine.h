#ifndef _IO_ENGINE_H_
#define _IO_ENGINE_H_
//#include "stdext.h"
#include "netmod.h"
#include "mutex.h"
#include <set>
#include <map>

struct IConn;
class IoEngine
{
public:
    IoEngine();
    ~IoEngine();

public:
    static IoEngine* Instance();
	static void Release();

public:
    bool    init();
    void    run(void*);
    void    stop();
    void    setEvent(IConn* pConn, SOCKET s, uint16_t opt, bool bAdd = true);

private:
    void    _onRecv(SOCKET s);
    void    _onSend(SOCKET s);

private:
    static IoEngine*        m_pInstance;
    volatile long           m_nStopping;			
    SOCKET                  m_uMaxFd;
    std::set<SOCKET>        m_readSoSet;
    std::set<SOCKET>        m_writeSoSet;
    std::map<SOCKET, IConn*> m_connMap;
	unsigned int			m_uDelayCheck;
};

#endif

