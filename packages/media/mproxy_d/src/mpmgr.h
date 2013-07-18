#ifndef MPMGR_H
#define MPMGR_H

#include "config.h"
#include "mpconfig.h"
#include <map>
#include <set>
#include <string>

class NetLoop;
class MsgHandler;
class Msg;
class MsgPool;
class BlackList;
class MsgCacheMgr;
class LoadMgr;
class Report;
class Session;
class SessMgr;

/**
 * center controller, and also interface for main.cpp.
 */
class MPMgr {
public:
	MPMgr(const MPConfig& config);
	~MPMgr();

public:	
	void		createSession(const std::string& stream);
	void		deleteSesssion(const std::string& stream);
	void		joinSession(int sid, int uid, const sockaddr_in& addr);

	void		push(int linkid, int sid, int from, Msg* msg);
	void		run();
	void		schedule();	

public:
	inline MPConfig*	getConfig() { return &m_config; }	//never keep a local copy.
	inline MsgHandler*	getHandler() { return m_pHandler; }
	inline NetLoop*		getLooper() { return m_pLooper; }
	inline MsgPool*		getMsgPool() { return m_pMsgPool; }
	inline BlackList*	getBlackList() { return m_pBlackList; }
	inline MsgCacheMgr*	getCacheMgr() { return m_pCacheMgr; }
	inline LoadMgr*		getLoadMgr() { return m_pLoadMgr; }
	inline SessMgr*		getSessMgr() { return m_pSessMgr; }

private:
	MPConfig	m_config;
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	MsgPool*	m_pMsgPool;
	BlackList*	m_pBlackList;
	MsgCacheMgr*m_pCacheMgr;
	LoadMgr*	m_pLoadMgr;
	Report*		m_pReport;
	SessMgr*	m_pSessMgr;
};

#endif
