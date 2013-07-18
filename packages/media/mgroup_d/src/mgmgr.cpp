#include "mgmgr.h"
#include "netloop.h"
#include "msghandler.h"
#include "proxymgr.h"
#include "linkmgr.h"

MGMgr::MGMgr(const MGConfig& config) 
	: m_config(config)
{
	m_pLooper = new NetLoop(this, config.mc_ip, config.mc_port, config.lt_ip, config.lt_port, config.port);
	m_pProxyMgr = new ProxyMgr();
	m_pHandler = new MsgHandler(this);
	m_pLinkMgr = new LinkMgr();
}

MGMgr::~MGMgr() {
	//[TBD] delete the objects, later.
	if( m_pLinkMgr ) {
		delete m_pLinkMgr;
	}
}

void	MGMgr::run() {
	m_pLooper->run();
}

void	MGMgr::report() {
	char hostname[65] = {'\0'};
	int load = 0;
	int load_max = 0;
	
	gethostname(hostname, 64);
	m_pProxyMgr->getLoadInfo(&load, &load_max);

	PMCReportReq report;
	report.name = hostname;
	report.ip = m_config.ip;
	report.port = m_config.port;
	report.isp = m_config.isp;
	report.area = m_config.area;
	report.load = load;
	report.load_max = load_max;


	Pack pk(SVID_MCENTER, PMCReportReq::uri);
	report.marshall(pk);
	pk.pack();

	m_pLooper->sendMC(pk.getBuf(), pk.getLen());
}

void	MGMgr::onTimer() {
	//clean the groups with no heartbeat for 5 seconds:
	int time = m_pLooper->getSystemTime();
	if( time == 0 ) {
		LOG(TAG_MGROUP, "MGMgr::onTimer, invalid time==0.");
		return; 
	}

	report();
}

