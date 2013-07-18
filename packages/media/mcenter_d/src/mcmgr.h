#ifndef MCMGR_H
#define MCMGR_H

#include "config.h"
#include "mcconfig.h"

class NetLoop;
class MsgHandler;
class GrpMgr;
class IspInfo;
class Statistic;
class LinkMgr;
class MCMgr {
public:
	MCMgr(const MCConfig& config);
	~MCMgr();

public:
	void		run();
	void		report();	//report to deamon.
	void		onTimer();

public:
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }
	inline GrpMgr*		getGrpMgr() { return m_pGrpMgr; }	
	inline IspInfo*		getIspInfo() { return m_pIspInfo; }
	inline Statistic*	getStatistic() { return m_pStatistic; }
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	GrpMgr*		m_pGrpMgr;
	IspInfo*	m_pIspInfo;
	Statistic*	m_pStatistic;
	LinkMgr*	m_pLinkMgr;
	MCConfig	m_config;
	int			m_nSeq;
};

#endif
