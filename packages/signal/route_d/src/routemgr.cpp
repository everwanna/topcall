#include "routemgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "linkmgr.h"

RouteMgr::RouteMgr(const DispConfig& config)
	: m_config(config)
{
	m_pLooper = new NetLoop(this, m_config.disp_ip, m_config.disp_port, m_config.port);
	m_pHandler = new MsgHandler(this);
	m_pLinkMgr = new LinkMgr();
	m_nSeq = 0;
}

RouteMgr::~RouteMgr() {
	//[TBD] delete the objects, later.
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pLinkMgr ) {
		delete m_pLinkMgr;
	}
}

void	RouteMgr::run() {
	LOG(TAG_DISPATCHER, "login_d, running now....");
	m_pLooper->run();
	LOG(TAG_DISPATCHER, "login_d, exiting now....");
}

void	RouteMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_DISPATCHER, "RouteMgr::onTimer, invalid time==0.");
		return; 
	}

	//
	//[TBD] what about other links, links from mgroup?
	//Seems no need HB for mgroup, the packages should be quite frequently.
	//
	if( m_nSeq % LINK_CHECK_INTERVAL == 0 ) {
		
	}

	m_nSeq++;
}
