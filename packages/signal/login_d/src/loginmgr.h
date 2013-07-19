#ifndef _LOGINMGR_H_
#define _LOGINMGR_H_

#include "config.h"
#include "loginconfig.h"

class NetLoop;
class MsgHandler;
class LTable;
class LinkMgr;
class MongoLink;
class UInfoMgr;
class LoginMgr {
public:
	LoginMgr(const LoginConfig& config);
	~LoginMgr();

public:
	void		run();
	void		onTimer();

public:
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }	
	inline LTable*		getLTable() { return m_pLTable; }
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }	
	inline MongoLink*	getMongo() { return m_pMongo; }
	inline UInfoMgr*	getUInfoMgr() { return m_pUInfoMgr; }
	inline LoginConfig*	getConfig() { return &m_config; }		//yeah, take care here.

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;	
	LTable*		m_pLTable;
	LinkMgr*	m_pLinkMgr;	
	MongoLink*	m_pMongo;
	UInfoMgr*	m_pUInfoMgr;
	LoginConfig	m_config;
	int			m_nSeq;
};

#endif


