#include "msghandler.h"
#include "routemgr.h"
#include "netloop.h"
#include "linkmgr.h"
#include "link.h"
#include "uinfo.h"
#include "mongolink.h"

using namespace login;
MsgHandler::MsgHandler(RouteMgr* mgr) 
	: m_pMgr(mgr)
{
	m_pBuffer = new char[1024];
	m_pPack = new Pack();
}

MsgHandler::~MsgHandler() {
	if( m_pBuffer ) {
		delete[] m_pBuffer;
	}
}

void	MsgHandler::handle(int linkid, const char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MCENTER, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	Unpack up(msg, len);
	up.popHead();

	int svid = up.getSvid();
	if( svid == SVID_LOGIN ) {
		switch( svid ) {
		case URI_REGDISP_REQ:
			onRegDispReq(linkid, &up);
			break;
		case URI_SEND_REQ:
			onSendReq(linkid, &up);
			break;
		}
	} else if( svid == SVID_PUSH ) {
		switch( svid ) {
		case URI_PUSH_MSG:
			onPushMsg(linkid, &up);
			break;
		}
	}
}

void	MsgHandler::onRegDispReq(int linkid, Unpack* up) {
	PRegDispReq req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "reg disp req, dispatcher=%s", req.dispatcher.c_str());
	PRegDispRes res;
	res.dispatcher = req.dispatcher;
	res.router = m_pMgr->getConfig()->name;

	m_pPack->reset(SVID_LOGIN, PRegDispRes::uri);
	res.marshall(*m_pPack);
	m_pPack->pack();

	m_pMgr->getLinkMgr()->send( linkid, m_pPack->getBuf(), m_pPack->getLen() );
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "send req, uid=%d, peer=%d, seq=%d.", req.uid, req.peer, req.seq);
	UInfo* uinfo = m_pMgr->getMongo()->query( req.peer );
	if( uinfo == NULL ) {
		LOG(TAG_ROUTER, "user offline, peer=%d, seq=%d.", req.peer, req.seq);
		//[TBD] need to save the packet for some time, and resend...
		return;
	}

	PPushMsg pmsg;
	pmsg.topic = uinfo->router;
	pmsg.payload.assign( up->getBuf(), up->getLen() );

	m_pPack->reset(SVID_PUSH, PPushMsg::uri);
	pmsg.marshall(*m_pPack);
	m_pPack->pack();
	m_pMgr->getLooper()->sendPush( m_pPack->getBuf(), m_pPack->getLen() );
}

void	MsgHandler::onPushMsg(int linkid, Unpack* up) {
	PPushMsg req;
	req.unmarshall(*up);

	LOG(TAG_ROUTER, "push msg for topic : %s", req.topic.c_str());
	handle(linkid, req.payload.c_str(), req.payload.length());
}

