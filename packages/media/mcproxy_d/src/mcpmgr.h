#ifndef MCPMGR_H
#define MCPMGR_H

#include "config.h"

class NetLoop;
class MsgHandler;
class Cache;
class PendList;
class Statistic;
class MCPMgr {
public:
	MCPMgr(const char* mc_ip, short mc_port, short port);
	~MCPMgr();

public:
	void		run();
	void		onTimer();

public:
	NetLoop*	getLooper() { return m_pLooper; }
	MsgHandler*	getHandler() { return m_pHandler; }	
	Cache*		getCache() { return m_pCache; }
	PendList*	getPendList() { return m_pPendList; }
	Statistic*	getStatistic() { return m_pStatistic; }

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;	
	Cache*		m_pCache;
	PendList*	m_pPendList;
	Statistic*	m_pStatistic;
	int			m_nTimerSeq;
};

#endif
