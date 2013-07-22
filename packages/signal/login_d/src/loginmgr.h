#ifndef _LOGINMGR_H_
#define _LOGINMGR_H_

#include "config.h"
#include "loginconfig.h"

class NetLoop;
class MsgHandler;
class LTable;
class LinkMgr;
class MongoLink;
class MsgResender;
class Seq;
class UidSync;
class LoginMgr {
public:
	LoginMgr(const LoginConfig& config);
	~LoginMgr();

public:
	void		run();
	void		onTimer();

public:
	void		setRouter(const std::string& router) { m_strRouter = router; }
	void		setDispatcher(const std::string& dispatcher) {  m_strDispatcher = dispatcher; }
	std::string getRouter() { return m_strRouter; }
	std::string getDispatcher() { return m_strDispatcher; }

public:
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgHandler*	getHandler() { return m_pHandler; }	
	inline LTable*		getLTable() { return m_pLTable; }
	inline LinkMgr*		getLinkMgr() { return m_pLinkMgr; }	
	inline MongoLink*	getMongo() { return m_pMongo; }
	inline MsgResender*	getResender() { return m_pResender; }
	inline Seq*			getSeq() { return m_pSeq; }
	inline UidSync*		getUidSync() { return m_pUidSync; }
	inline LoginConfig*	getConfig() { return &m_config; }		//yeah, take care here.

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;	
	LTable*		m_pLTable;
	LinkMgr*	m_pLinkMgr;	
	MongoLink*	m_pMongo;
	MsgResender*m_pResender;
	Seq*		m_pSeq;
	UidSync*	m_pUidSync;
	LoginConfig	m_config;
	int			m_nSeq;
	std::string	m_strRouter;
	std::string m_strDispatcher;
};

#endif
