#include "msghandler.h"
#include "dispmgr.h"
#include "netloop.h"
#include "linkmgr.h"

using namespace login;

MsgHandler::MsgHandler(DispMgr* mgr) 
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
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	case URI_SYNCUIDS_REQ:
		onSyncUidsReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_DISPATCHER, "send from uid %d to peer %d", req.uid, req.peer);
	if( m_pLoginMgr->getLinkMgr()->hasUid(req.peer) ) {
		m_pLoginMgr->getLinkMgr()->send(req.peer, up->getBuf(), up->getLen() );

	} else {
		m_pLoginMgr->getLooper()->sendRouter(up->getBuf(), up->getLen());
	}
}

void	MsgHandler::onSyncUidsReq(int linkid, Unpack* up) {
	PSyncUidsReq req;
	req.unmarshall(*up);

	for( size_t i=0; i<req.uids.size(); i++ ) {
		m_pLoginMgr->getLinkMgr()->setUid( req.uids[i], linkid );
	}
}

