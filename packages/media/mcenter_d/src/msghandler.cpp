#include "msghandler.h"
#include "mcmgr.h"
#include "grpmgr.h"
#include "grpinfo.h"
#include "netloop.h"
#include "ispinfo.h"
#include "statistic.h"
#include "linkmgr.h"

MsgHandler::MsgHandler(MCMgr* mgr) 
	: m_badGrpPack(SVID_MCENTER, PMCPreAllocRes::uri)
{
	m_pMCMgr = mgr;

	PMCPreAllocRes res;
	res.res = RES_FAIL;
	res.ip = "";
	res.port = 0;
	
	res.marshall(m_badGrpPack);
	m_badGrpPack.pack();
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MCENTER, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	Unpack up(msg, len);
	up.popHead();
	
	if( up.getSvid() != SVID_MCENTER ) {
		LOG(TAG_MCENTER, "MsgHandler::handle, svid!=SVID_MCENTER, svid=%d", up.getSvid());
		//[TBD]
		//Should consider the blocklist.
		return;
	}

	switch(up.getUri() ) {
	case URI_MCENTER_REPORT_REQ:
		onMCReportReq(linkid, &up);
		break;
	case URI_MCENTER_PREALLOC_REQ:
		onMCPreAllocReq(linkid, &up);
		break;
	case URI_MCENTER_PING:
		onMCPing(linkid, &up);
		break;
	}
}

void	MsgHandler::onMCReportReq(int linkid, Unpack* up) {
#ifdef _DEBUG
	//LOG(TAG_MCENTER, "MsgHandler::onMCReportReq, linkid=%d", linkid);
#endif

	PMCReportReq req;
	req.unmarshall(*up);

	//[TBD] validate the package.
	if( !m_pMCMgr->getIspInfo()->isValidIsp(req.isp) ) {
		LOG(TAG_MCENTER, "MsgHandler::onMCReportReq, invalid linkid/isp=%d, %d", linkid, req.isp);
		return;
	}

	//handle the package.
	m_pMCMgr->getGrpMgr()->add(linkid, req.name, req.ip, req.port, req.isp, req.area, req.load, req.load_max, req.access_url);	

	//yeah, report also update stamp:
	int time = m_pMCMgr->getLooper()->getSystemTime();
	m_pMCMgr->getLinkMgr()->update(linkid, time);
}

/**
 * onAllocProxyReq
 *
 * The command need to be forward to mgroup. mgroup allocate a proxy and returns to mcenter.
 * mcenter will forward the response back to client.
 */
void	MsgHandler::onMCPreAllocReq(int linkid, Unpack* up) {
	PMCPreAllocReq req;
	req.unmarshall(*up);

	//can be a ping:
	int time = m_pMCMgr->getLooper()->getSystemTime();
	m_pMCMgr->getLinkMgr()->update(linkid, time);

	// validate the package.
	if( !m_pMCMgr->getIspInfo()->isValidIsp(req.isp1) ||
		!m_pMCMgr->getIspInfo()->isValidIsp(req.isp2)
		) {
		LOG(TAG_MCENTER, "MsgHandler::onMCReportReq, invalid linkid/isp1/isp2=%d, %d, %d", linkid, req.isp1, req.isp2);
		PMCPreAllocRes res;
		res.res = RES_INVAID_ISP;
		res.isp1 = req.isp1;
		res.area1 = req.area1;
		res.isp2 = req.isp2;
		res.area2 = req.area2;
		res.num = 0;
		//res.ip = grp->getIp();
		res.port = 0;

		Pack pk(SVID_MCENTER, PMCPreAllocRes::uri);
		res.marshall(pk);
		pk.pack();
		m_pMCMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());
		
		return;
	}

	GrpInfo* grp = m_pMCMgr->getGrpMgr()->fetchGroup(req.isp1, req.area1, req.isp2, req.area2, req.num);
	if( grp == NULL ) {
		LOG(TAG_MCENTER, "MsgHandler::onMCPreAllocReq, find group failed for isp1/area1/isp2/area2=%d,%d,%d,%d", 
			req.isp1, req.area1, req.isp2, req.area2);
		//[TBD] send back response.		
		PMCPreAllocRes res;
		res.res = RES_NO_GROUP;
		res.isp1 = req.isp1;
		res.area1 = req.area1;
		res.isp2 = req.isp2;
		res.area2 = req.area2;
		res.num = 0;
		//res.ip = grp->getIp();
		res.port = 0;

		Pack pk(SVID_MCENTER, PMCPreAllocRes::uri);
		res.marshall(pk);
		pk.pack();
		m_pMCMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());

	} else {	
		PMCPreAllocRes res;
		res.res = RES_OK;
		res.isp1 = req.isp1;
		res.area1 = req.area1;
		res.isp2 = req.isp2;
		res.area2 = req.area2;
		res.num = req.num;
		res.ip = grp->getIp();
		res.port = grp->getPort();

		Pack pk(SVID_MCENTER, PMCPreAllocRes::uri);
		res.marshall(pk);
		pk.pack();
		m_pMCMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());

		LOG(TAG_MCENTER, "MsgHandler::onMCPreAllocReq, prealloc for isp1/area1/isp2/area2/ip=%d,%d,%d,%d,%s",
			req.isp1, req.area1, req.isp2, req.area2, res.ip.c_str());

		//add up the GrpInfo::m_nLoadAllocated.
		grp->addAllocated(req.num);
	}
}

void	MsgHandler::onMCPing(int linkid, Unpack* up) {
	int time = m_pMCMgr->getLooper()->getSystemTime();
	m_pMCMgr->getLinkMgr()->update(linkid, time);
}
