#ifndef _MGMGR_H_
#define _MGMGR_H_

#include "config.h"
#include "mgconfig.h"

class NetLoop;
class MsgHandler;
class ProxyMgr;
class LinkMgr;
class MGMgr {
public:
	MGMgr(const MGConfig& config);
	~MGMgr();

public:
	void		run();
	void		report();
	void		onTimer();

public:
	inline NetLoop*	getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }
	inline ProxyMgr*	getProxyMgr() { return m_pProxyMgr; }
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }

private:
	MGConfig	m_config;
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	ProxyMgr*	m_pProxyMgr;
	LinkMgr*	m_pLinkMgr;
};

#endif
