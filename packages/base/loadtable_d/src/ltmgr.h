#ifndef _LTMGR_H_
#define _LTMGR_H_

#include "config.h"
#include "ltconfig.h"

class NetLoop;
class MsgHandler;
class LTable;
class LinkMgr;
class SlaveMgr;
class LTMgr {
public:
	LTMgr(const LTConfig& config);
	~LTMgr();

public:
	void		run();
	void		onTimer();

public:
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }	
	inline LTable*		getLTable() { return m_pLTable; }
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }
	inline SlaveMgr*	getSlaveMgr() { return m_pSlaveMgr; }
	inline LTConfig*	getConfig() { return &m_config; }		//yeah, take care here.

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;	
	LTable*		m_pLTable;
	LinkMgr*	m_pLinkMgr;
	SlaveMgr*	m_pSlaveMgr;
	LTConfig	m_config;
	int			m_nSeq;
};

#endif


