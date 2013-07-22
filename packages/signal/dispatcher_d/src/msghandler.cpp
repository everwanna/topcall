#include "msghandler.h"
#include "dispmgr.h"
#include "netloop.h"
#include "linkmgr.h"

using namespace login;

MsgHandler::MsgHandler(DispMgr* mgr) 
	: m_pDispMgr(mgr)
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
	case URI_REGPROXY_REQ:
		onRegProxyReq(linkid, &up);
		break;
	case URI_REGDISP_RES:
		onRegDispRes(linkid, &up);
		break;
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	case URI_SYNCUIDS_REQ:
		onSyncUidsReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onRegProxyReq(int linkid, Unpack* up) {
	PRegProxyReq req;
	req.unmarshall(*up);

	LOG(TAG_DISPATCHER, "reg proxy name=%s.", req.proxy.c_str());
	PRegProxyRes res;
	res.proxy = req.proxy;
	res.router = m_pDispMgr->getRouter();
	res.dispatcher = m_pDispMgr->getConfig()->name;

	Pack pk(SVID_LOGIN, PRegProxyRes::uri);
	res.marshall(pk);
	pk.pack();

	m_pDispMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());
}

void	MsgHandler::onRegDispRes(int linkid, Unpack* up) {
	PRegDispRes res;
	res.unmarshall(*up);

	LOG(TAG_DISPATCHER, "reg dispatcher res, router=%s", res.router.c_str());
	if( res.dispatcher != m_pDispMgr->getConfig()->name ) {
		LOG(TAG_DISPATCHER, "MsgHandler::onRegDispRes, disp != config.name. proxy=%s, config.name=%s", 
			res.dispatcher.c_str(), m_pDispMgr->getConfig()->name.c_str());
	}

	m_pDispMgr->setRouter(res.router);
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_DISPATCHER, "send from uid %d to peer %d", req.uid, req.peer);
	if( m_pDispMgr->getLinkMgr()->hasUid(req.peer) ) {
		m_pDispMgr->getLinkMgr()->send(req.peer, up->getBuf(), up->getLen() );

	} else {
		m_pDispMgr->getLooper()->sendRouter(up->getBuf(), up->getLen());
	}
}

void	MsgHandler::onSyncUidsReq(int linkid, Unpack* up) {
	PSyncUidsReq req;
	req.unmarshall(*up);

	for( size_t i=0; i<req.uids.size(); i++ ) {
		m_pDispMgr->getLinkMgr()->setUid( req.uids[i], linkid );
	}
}

