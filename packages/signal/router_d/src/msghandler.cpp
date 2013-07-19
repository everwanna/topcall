#include "msghandler.h"
#include "routemgr.h"
#include "netloop.h"
#include "linkmgr.h"
#include "link.h"
#include "routerlinkmgr.h"

using namespace login;

MsgHandler::MsgHandler(RouteMgr* mgr) 
	: m_pLoginMgr(mgr)
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
	case URI_REGROUTER_REQ:
		onRegRouterReq(linkid, &up);
		break;
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onRegRouterReq(int linkid, Unpack* up) {
	PRegRouterReq req;
	req.unmarshall(*up);

	LOG(TAG_DISPATCHER, "router register, name=%s", req.name.c_str());
	m_pLoginMgr->getRouterLinkMgr()->set(req.name, linkid);	
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_DISPATCHER, "send from uid %d to peer %d", req.uid, req.peer);
	if( m_pLoginMgr->getLinkMgr()->hasUid(req.peer) ) {
		//if we already know where to send:
		m_pLoginMgr->getLinkMgr()->send(req.peer, up->getBuf(), up->getLen() );

	} else {
		//query the online db, and send:
		std::string router = "";
		int linkid = m_pLoginMgr->getRouterLinkMgr()->get(router);
		if( linkid <= 0 ) {
			LOG(TAG_DISPATCHER, "MsgHandler::onSendReq, dispatcher doesn't exist, name=%s", router);
			return;
		}

		m_pLoginMgr->getLinkMgr()->send(linkid, up->getBuf(), up->getLen());
	}
}

