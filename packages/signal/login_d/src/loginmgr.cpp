#include "loginmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "linkmgr.h"
#include "mongolink.h"
#include "uinfomgr.h"
#include "msgresender.h"

LoginMgr::LoginMgr(const LoginConfig& config)
	: m_config(config)
{
	m_pLooper = new NetLoop(this, m_config.disp_ip, m_config.disp_port, m_config.port);
	m_pHandler = new MsgHandler(this);
	m_pLinkMgr = new LinkMgr(this);
	m_pMongo = new MongoLink();
	m_pMongo->connect( m_config.udb_ip, m_config.udb_port);
	m_pUInfoMgr = new UInfoMgr();
	m_pResender = new MsgResender(this);

	m_nSeq = 0;
}

LoginMgr::~LoginMgr() {
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
	if( m_pMongo ) {
		delete m_pMongo;
	}
	if( m_pUInfoMgr ) {
		delete m_pUInfoMgr;
	}
}

void	LoginMgr::run() {
	LOG(TAG_LOGIN, "login_d, running now....");
	m_pLooper->run();
	LOG(TAG_LOGIN, "login_d, exiting now....");
}

void	LoginMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_LOGIN, "LoginMgr::onTimer, invalid time==0.");
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
