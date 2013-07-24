#include "msghandler.h"
#include "routemgr.h"
#include "netloop.h"
#include "linkmgr.h"
#include "link.h"

using namespace login;
MsgHandler::MsgHandler(RouteMgr* mgr) 
	: m_pMgr(mgr)
{
	m_pBuffer = new char[1024];
}

MsgHandler::~MsgHandler() {
	if( m_pBuffer ) {
		delete[] m_pBuffer;
	}
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



