#include "mcmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "grpmgr.h"
#include "ispinfo.h"
#include "statistic.h"
#include "linkmgr.h"

MCMgr::MCMgr(const MCConfig& config) 
	: m_config(config)
{
	m_pLooper = new NetLoop(this, config.deamon_ip.c_str(), config.deamon_port, config.port);
	m_pHandler = new MsgHandler(this);
	m_pGrpMgr = new GrpMgr(this);
	m_pIspInfo = new IspInfo();
	m_pStatistic = new Statistic(this);
	m_pLinkMgr = new LinkMgr();
	m_nSeq = 0;
}

MCMgr::~MCMgr() {
	//[TBD] delete the objects, later.
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pGrpMgr ) {
		delete m_pGrpMgr; 
	}
	if( m_pIspInfo ) {
		delete m_pIspInfo; 
	}
	if( m_pStatistic ) {
		delete m_pStatistic;
	}
	if( m_pLinkMgr ) {
		delete m_pLinkMgr;
	}
}

void	MCMgr::run() {
	LOG(TAG_MCENTER, "mcenter running now....");
	m_pLooper->run();
	LOG(TAG_MCENTER, "mcenter exiting now....");
}

void	MCMgr::report() {
	PDReport req;
	req.name = m_config.name;	
	req.ip = m_config.ip;
	req.port = m_config.port;
	req.svid = SVID_MCENTER;
	req.isp = m_config.isp;
	req.area = m_config.area;
	req.load = 0;
	req.load_max = 0;	
	
	Pack pk(SVID_DEAMON, PDReport::uri);
	req.marshall(pk);
	pk.pack();

	m_pLooper->sendDM(pk.getBuf(), pk.getLen());
}

void	MCMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_MCENTER, "MCMgr::onTimer, invalid time==0.");
		return; 
	}

	m_pGrpMgr->removeExpire(time);
	m_pLinkMgr->removeExpire(time);

	if( m_nSeq % REPORT_INTERVAL == 0 ) {
		report();
	}

	if( m_nSeq % 30 == 0 ) {
		LOG(TAG_MCENTER, "alive for %d seconds", m_nSeq);
	}
	m_nSeq++;
}
