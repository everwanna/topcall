#ifndef _ROUTEMGR_H_
#define _ROUTEMGR_H_

#include "config.h"
#include "dispconfig.h"

class NetLoop;
class MsgHandler;
class LinkMgr;
class RouteMgr {
public:
	RouteMgr(const DispConfig& config);
	~RouteMgr();

public:
	void		run();
	void		onTimer();

public:
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }	
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }
	inline DispConfig*	getConfig() { return &m_config; }		//yeah, take care here.

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;	
	LinkMgr*	m_pLinkMgr;
	DispConfig	m_config;
	int			m_nSeq;
};

#endif


