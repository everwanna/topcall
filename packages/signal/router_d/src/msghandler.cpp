#include "msghandler.h"
#include "routemgr.h"
#include "netloop.h"
#include "linkmgr.h"
#include "link.h"
#include "routerlinkmgr.h"

using namespace login;

MsgHandler::MsgHandler(RouteMgr* mgr) 
	: m_pMgr(mgr)
{
	m_pBuffer = new char[1024];
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MCENTER, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	Unpack up(msg, len);
	up.popHead();

	switch( up.getUri() ) {
	case URI_REGDISP_REQ:
		onRegDispReq(linkid, &up);
		break;
	case URI_REGROUTER_REQ:
		onRegRouterReq(linkid, &up);
		break;
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onRegDispReq(int linkid, Unpack* up) {
	PRegDispReq req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "reg disp req, dispatcher=%s", req.dispatcher.c_str());
	PRegDispRes res;
	res.dispatcher = req.dispatcher;
	res.router = m_pMgr->getConfig()->name;

	Pack pk(SVID_LOGIN, PRegDispRes::uri);
	res.marshall(pk);
	pk.pack();

	m_pMgr->getLinkMgr()->send( linkid, pk.getBuf(), pk.getLen() );
}

void	MsgHandler::onRegRouterReq(int linkid, Unpack* up) {
	PRegRouterReq req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "router register, name=%s", req.name.c_str());
	m_pMgr->getRouterLinkMgr()->set(req.name, linkid);	
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "send from uid %d to peer %d", req.uid, req.peer);
	if( m_pMgr->getLinkMgr()->hasUid(req.peer) ) {
		//if we already know where to send:
		m_pMgr->getLinkMgr()->send(req.peer, up->getBuf(), up->getLen() );

	} else {
		//query the online db, and send:
		std::string router = "";
		int linkid = m_pMgr->getRouterLinkMgr()->get(router);
		if( linkid <= 0 ) {
			LOG(TAG_ROUTER, "MsgHandler::onSendReq, dispatcher doesn't exist, name=%s", router);
			return;
		}

		m_pMgr->getLinkMgr()->send(linkid, up->getBuf(), up->getLen());
	}
}


