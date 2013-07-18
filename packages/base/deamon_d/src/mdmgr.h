#ifndef MDMGR_H
#define MDMGR_H

#include "config.h"

class NetLoop;
class MsgHandler;
class SvcMgr;

class MDMgr {
public:
	MDMgr(short port);
	~MDMgr();

public:
	void		run();
	void		onTimer();

public:
	NetLoop*	getLooper() { return m_pLooper; }
	MsgHandler*	getHandler() { return m_pHandler; }
	SvcMgr*		getSvcMgr() { return m_pSvcMgr; }

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	SvcMgr*		m_pSvcMgr;
	int			m_nSeq;
};

#endif