#include "msghandler.h"
#include "mgmgr.h"
#include "proxymgr.h"
#include "proxyinfo.h"
#include "netloop.h"
#include "linkmgr.h"
#include "link.h"

MsgHandler::MsgHandler(MGMgr* mgr) 
	: m_pMGMgr(mgr)
{	
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MGROUP, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	/*
	 * impossible because recvfrom use NET_BUFFER_SIZE as the recv buffer length.
	if( len > NET_BUFFER_SIZE ) {
		LOG("MsgHandler::handle, msg too long, len=%d", len);
		return;
	}
	*/

	Unpack up(msg, len);
	up.popHead();
	
	if( up.getSvid() != SVID_MGROUP ) {
		LOG(TAG_MGROUP, "MsgHandler::handle, svid!=SVID_MGROUP, svid=%d", up.getSvid());
		//[TBD]
		//Should consider the blocklist.
		return;
	}

	switch(up.getUri() ) {
	case URI_MGROUP_REPORT_REQ:
		onReportReq(linkid, &up);
		break;
	case URI_MGROUP_ALLOC_REQ:
		onAllocProxyReq(linkid, &up);
		break;
	}
}

/**
 * [TBD]
 * Do I really need to unpack PMGReportReq and pack PLTReport?
 * The content is almost the same.
 *
 * I have no idea if there maybe difference later, let it like this for now.
 */
void	MsgHandler::onReportReq(int linkid, Unpack* up) {
	PMGReportReq req;
	req.unmarshall(*up);

	m_pMGMgr->getProxyMgr()->add(linkid, req.name, req.ip, req.port, (req.udp==1), req.load, req.load_max);

	//and send the data to loadtable:
	PLTReport report;
	report.key = req.name;
	report.ip = req.ip;
	report.port = req.port;
	report.load = req.load;
	report.load_max = req.load_max;
	Pack pk(SVID_LOADTABLE, PLTReport::uri);
	report.marshall(pk);
	pk.pack();
	m_pMGMgr->getLooper()->sendLT(pk.getBuf(), pk.getLen());
}

void	MsgHandler::onAllocProxyReq(int linkid, Unpack* up) {		

	PMGAllocReq req;
	req.unmarshall(*up);	

	ProxyInfo* info = m_pMGMgr->getProxyMgr()->getFreeProxy();
	PMGAllocRes res;
	if( info == NULL ) {
		res.stream = req.stream;
		res.res  = RES_FAIL;
	} else {
		res.stream = req.stream;
		res.cookie = req.stream;	//yeah, we need a real passcode.
		res.res  = RES_OK;
		res.name = info->getName();
		res.ip = info->getIp();
		res.port = info->getPort();
		res.udp = info->getUdp();
		res.load = info->getLoad();
		res.load_max = info->getLoadMax();
	}
	LOG(TAG_MGROUP, "MsgHandler:;onAllocProxyReq, linkid/stream/res/ip/port=%d, %s, %d, %s, %d", 
		linkid, req.stream.c_str(), res.res, res.ip.c_str(), res.port);

	//send the packet back:
	Pack pk(SVID_MGROUP, PMGAllocRes::uri);
	res.marshall(pk);
	pk.pack();

	//m_pMPGMgr->getLooper()->sendMC(pk.getBuf(), pk.getLen());
	m_pMGMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());

	//send a notify to the mproxy:
	if( info != NULL ) {
		LOG(TAG_MGROUP, "MsgHandler:;onAllocProxyReq, send msg to mproxy, linkid=%d", info->getLinkId());
		m_pMGMgr->getLinkMgr()->send(info->getLinkId(), pk.getBuf(), pk.getLen());
	}
}

