#include "ltmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "ltable.h"
#include "linkmgr.h"
#include "slavemgr.h"

LTMgr::LTMgr(const LTConfig& config)
	: m_config(config)
{
	m_pLooper = new NetLoop(this, m_config.master_ip, m_config.master_port, m_config.port);
	m_pHandler = new MsgHandler(this);
	m_pLTable = new LTable();
	m_pLinkMgr = new LinkMgr();
	m_pSlaveMgr = new SlaveMgr(this);
	m_nSeq = 0;
}

LTMgr::~LTMgr() {
	//[TBD] delete the objects, later.
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pLTable ) {
		delete m_pLTable;
	}
	if( m_pLinkMgr ) {
		delete m_pLinkMgr;
	}
	if( m_pSlaveMgr ) {
		delete m_pSlaveMgr;
	}
}

void	LTMgr::run() {
	LOG(TAG_LTABLE, "loadtable running now....");
	m_pLooper->run();
	LOG(TAG_LTABLE, "loadtable exiting now....");
}

void	LTMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_LTABLE, "LTMgr::onTimer, invalid time==0.");
		return; 
	}

	//
	//[TBD] what about other links, links from mgroup?
	//Seems no need HB for mgroup, the packages should be quite frequently.
	//
	if( m_nSeq % LINK_CHECK_INTERVAL == 0 ) {
		m_pSlaveMgr->removeExpired( time );

		//send the heartbeat if this is a alave:
		if( m_config.master_ip.length() != 0 && m_config.master_port != 0 ) {
			PLTPing ping;
			Pack pk(SVID_LOADTABLE, PLTPing::uri);
			ping.marshall(pk);
			pk.pack();

			m_pLooper->sendMaster(pk.getBuf(), pk.getLen());
		}
	}

	m_nSeq++;
}
