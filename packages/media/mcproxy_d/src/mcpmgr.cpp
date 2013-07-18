#include "mcpmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "cache.h"
#include "pendlist.h"
#include "statistic.h"

MCPMgr::MCPMgr(const char* mc_ip, short mc_port, short port) {
	m_pLooper = new NetLoop(this, mc_ip, mc_port, port);
	m_pHandler = new MsgHandler(this);
	m_pCache = new Cache(this);
	m_pPendList = new PendList();
	m_pStatistic = new Statistic(this);
	m_nTimerSeq = 0;
}

MCPMgr::~MCPMgr() {
	//[TBD] delete the objects, later.
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pCache ) {
		delete m_pCache;
	}
	if( m_pPendList ) {
		delete m_pPendList; 
	}
	if( m_pStatistic ) {
		delete m_pStatistic;
	}
}

void	MCPMgr::run() {
	LOG(TAG_MCPROXY, "mcproxy running now....");
	m_pLooper->run();
	LOG(TAG_MCPROXY, "mcproxy exiting now....");
}

void	MCPMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_MCPROXY, "MCMgr::onTimer, invalid time==0.");
		return; 
	}

	if( m_nTimerSeq%10 == 0 ) {
		m_pPendList->dump();
		m_pCache->dump();
		m_pStatistic->dump();
	}

	//clear every 3 seconds
	if( m_nTimerSeq%3 == 0 ) {
		m_pPendList->clearExpire(time);
		m_pCache->clearExpire(time);
	}

	//will it go out of boundry?
	//hit each seconds, we can do at least 1024*1024*1024*4/(60*60*24*365)=160 year??
	//I'm fine with this.
	m_nTimerSeq++;
}
