#include "report.h"
#include "mpmgr.h"
#include "loadmgr.h"
#include "mpconfig.h"
#include "netloop.h"

Report::Report(MPMgr* mgr) 
	: m_pMPMgr(mgr) 
{
}

Report::~Report() {
}

void	Report::report() {
	int	load = m_pMPMgr->getLoadMgr()->getLoad();
	int load_max = m_pMPMgr->getLoadMgr()->getMaxLoad();	
	MPConfig* config = m_pMPMgr->getConfig();

	PMGReportReq reg;
	reg.name = config->name;
	reg.ip = config->ip;
	reg.port = config->port;
	reg.udp = 1;
	reg.load = load;
	reg.load_max = load_max;
	sendReport(&reg);
}

void	Report::sendReport(PMGReportReq* reg) {
	Pack pk(SVID_MGROUP, PMGReportReq::uri);
	reg->marshall(pk);
	pk.pack();

	//::send(m_nSocket, pk.getBuf(), pk.getLen(), 0);
	m_pMPMgr->getLooper()->sendMG(pk.getBuf(), pk.getLen());
}

