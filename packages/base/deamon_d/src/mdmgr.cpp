#include "mdmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "svcmgr.h"

MDMgr::MDMgr(short port) {
	m_pLooper = new NetLoop(this, port);
	m_pHandler = new MsgHandler(this);
	m_pSvcMgr = new SvcMgr();
	m_nSeq = 0;
}

MDMgr::~MDMgr() {
	//[TBD] delete the objects, later.
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pSvcMgr ) {
		delete m_pSvcMgr; 
	}
}

void	MDMgr::run() {
	LOG(TAG_DEAMON, "MCMgr::run, running now....");
	m_pLooper->run();
	LOG(TAG_DEAMON, "MCMgr::run, exiting now....");
}

void	MDMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_DEAMON, "MCMgr::onTimer, invalid time==0.");
		return; 
	}

	//clean up expired server:
	if( m_nSeq%CHECK_INTERVAL == 0 ) {
		m_pSvcMgr->removeExpire( time );
	}

	m_nSeq++;
}