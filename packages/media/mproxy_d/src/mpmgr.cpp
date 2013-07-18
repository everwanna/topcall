#include "mpmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "msgpool.h"
#include "blacklist.h"
#include "msg.h"
#include "msgcachemgr.h"
#include "loadmgr.h"
#include "report.h"
#include "sessmgr.h"
#include "session.h"

MPMgr::MPMgr(const MPConfig& config)
	: m_config(config)
{
	m_pLooper = new NetLoop(this, config.mg_ip, config.mg_port, config.port);
	m_pHandler = new MsgHandler(this);
	m_pMsgPool = new MsgPool();
	m_pBlackList = new BlackList();
	m_pCacheMgr = new MsgCacheMgr();
	m_pLoadMgr = new LoadMgr(this, config.load_max);
	m_pReport = new Report(this);
	m_pSessMgr = new SessMgr(this);
}

MPMgr::~MPMgr() {
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pMsgPool ) {
		delete m_pMsgPool;
	}
	if( m_pBlackList ) {
		delete m_pBlackList;
	}
	if( m_pCacheMgr ) {
		delete m_pCacheMgr; 
	}
	if( m_pLoadMgr ) {
		delete m_pLoadMgr;
	}
	if( m_pReport ) {
		delete m_pReport; 
	}
	if( m_pSessMgr ) {
		delete m_pSessMgr;
	}
}

void	MPMgr::createSession(const std::string& stream) {
	LOG(TAG_MPROXY, "create session, stream=%s", stream.c_str());
}

void	MPMgr::deleteSesssion(const std::string& stream) {
	LOG(TAG_MPROXY, "delete session, stream=%s", stream.c_str());
}

void	MPMgr::joinSession(int sid, int uid, const sockaddr_in& addr) {
	LOG(TAG_MPROXY, "join session, sid=%d, uid=%d, ip/port=%d:%d", sid, uid, addr.sin_addr.s_addr, addr.sin_port);

}

void	MPMgr::push(int linkid, int sid, int from, Msg* msg) {
	Session* session = m_pSessMgr->getSession(sid);
	if( session == NULL )
		return;
	session->onMsg(linkid, sid, from, msg);
	m_pMsgPool->free(msg);
}

void	MPMgr::run() {
	LOG(TAG_MPROXY, "");
	LOG(TAG_MPROXY, "Starting network looper.");
	LOG(TAG_MPROXY, "Starting..");
	LOG(TAG_MPROXY, "Starting....");
	LOG(TAG_MPROXY, "Running.............");	
	LOG(TAG_MPROXY, "");
	if( m_pLooper ) {
		m_pLooper->run();
	}
	
	LOG(TAG_MPROXY, "");
	LOG(TAG_MPROXY, "Exiting network looper.");
	LOG(TAG_MPROXY, "Exiting....");
	LOG(TAG_MPROXY, "Exiting.............");
	LOG(TAG_MPROXY, "");
}


/**
 * The is the top level scheduler.
 * Currently the following jobs should be done:
 * 1, remove producers with no heartbeat for more than 2 mins.
 * 2, remove consumers with no heartbeat for more than 2 mins.
 * 3, report status to media center.
 */
void	MPMgr::schedule() 
{	
	static int seq = 0;
	//[TBD] Help, help...
	timeval tv;
	evutil_gettimeofday(&tv, NULL);
	
	if( (seq++)%STREAM_CHECK_INTERVAL == 0 ) {
		//LOG(TAG_MPROXY, "MPMgr::schedule, start clean up, time=%d.", tv.tv_sec);
		
		//m_pConsMgr->cleanup(tv.tv_sec);
	}
	m_pReport->report();

	if( seq%DUMP_INTERVAL == 0 ) {
		
	}
}
