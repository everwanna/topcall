#include "msghandler.h"
#include "loginmgr.h"
#include "netloop.h"
#include "linkmgr.h"

using namespace login;

MsgHandler::MsgHandler(LoginMgr* mgr) 
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

	switch(up.getUri() ) {
	case URI_LOGIN_REQ:
		onLoginReq(linkid, &up);
		break;
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onLoginReq(int linkid, Unpack* up) {
	PLoginReq req;
	req.unmarshall(*up);

	LOG(TAG_LOGIN, "login user with uid=%d, passport=%s.", req.uid, req.passport.c_str());
	//verify the user:


	//save the uid to LinkMgr.
	m_pLoginMgr->getLinkMgr()->setUid(req.uid, linkid);
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_LOGIN, "send msg to from %d to peer %d, with seq=.", req.uid, req.peer, req.seq);
	if( m_pLoginMgr->getLinkMgr()->hasUid(req.peer) ) {
		//best scenario, uid/peer on the same proxy:
		m_pLoginMgr->getLinkMgr()->sendByUid(req.peer, up->getBuf(), up->getLen());
		return;
	} else {
		//send to dispatcher:
		m_pLoginMgr->getLooper()->sendDispatcher(up->getBuf(), up->getLen());
	}
}

