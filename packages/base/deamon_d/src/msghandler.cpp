#include "msghandler.h"
#include "mdmgr.h"
#include "netloop.h"
#include "svcmgr.h"
#include "svcinfo.h"
#include "svclist.h"

MsgHandler::MsgHandler(MDMgr* mgr) {
	m_pMDMgr = mgr;
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	LOG(TAG_DEAMON, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif

	Unpack up(msg, len);
	up.popHead();
	
	if( up.getSvid() != SVID_DEAMON ) {
		LOG(TAG_DEAMON, "MsgHandler::handle, svid!=SVID_MCENTER, svid=%d", up.getSvid());
		//[TBD]
		//Should consider the blocklist.
		return;
	}

	switch(up.getUri() ) {
	case URI_MDEAMON_REPORT:
		onMDReport(linkid, &up);
		break;
	case URI_MDEAMON_QUERY_REQ:
		onMDQueryReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onMDReport(int linkid, Unpack* up) {
#ifdef _DEBUG
	//LOG(TAG_DEAMON, "MsgHandler::onMDReport, linkid=%d", linkid);
#endif

	PDReport req;
	req.unmarshall(*up);
	
	m_pMDMgr->getSvcMgr()->add(req.name, req.ip, req.port, req.svid, req.isp, req.area, req.load, req.load_max, 
		m_pMDMgr->getLooper()->getSystemTime());
}

void	MsgHandler::onMDQueryReq(int linkid, Unpack* up) {
#ifdef _DEBUG
	//LOG(TAG_DEAMON, "MsgHandler::onMDQueryReq, linkid=%d", linkid);
#endif
	PDQueryRes res;
	PDQueryReq req;
	req.unmarshall(*up);
	
	SvcList* list = m_pMDMgr->getSvcMgr()->query(req.svid);
	if( list == NULL ) {
		LOG(TAG_DEAMON, "MsgHandler::onMDQueryReq, find no svclist for svc=%d", req.svid);
		//[TBD] should send back a packet.
		res.res = RES_FAIL;		
	} else {
		list->getSvcInfos(res);
	}

	m_pMDMgr->getLooper()->send(linkid, PDQueryRes::uri, res);
}

