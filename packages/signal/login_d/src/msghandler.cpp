#include "msghandler.h"
#include "loginmgr.h"
#include "netloop.h"
#include "linkmgr.h"
#include "mongolink.h"
#include "uinfo.h"
#include "uinfomgr.h"

using namespace login;

MsgHandler::MsgHandler(LoginMgr* mgr) 
	: m_pLoginMgr(mgr)
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

	switch(up.getUri() ) {
	case URI_REGPROXY_RES:
		onRegProxyRes(linkid, &up);
		break;
	case URI_LOGIN_REQ:
		onLoginReq(linkid, &up);
		break;
	case URI_SEND_REQ:
		onSendReq(linkid, &up);
		break;
	}
}

void	MsgHandler::onRegProxyRes(int linkid, Unpack* up) {
	PRegProxyRes res;
	res.unmarshall(*up);

	LOG(TAG_LOGIN, "reg proxy res, dispatcher=%s, router=%s", res.dispatcher.c_str(), res.router.c_str());
	if( res.proxy != m_pLoginMgr->getConfig()->name ) {
		LOG(TAG_LOGIN, "MsgHandler::onRegProxyRes, proxy != config.name. proxy=%s, config.name=%s", res.proxy.c_str(), m_pLoginMgr->getConfig()->name.c_str());
	}

	m_pLoginMgr->setRouter(res.router);
	m_pLoginMgr->setDispatcher(res.dispatcher);
}

void	MsgHandler::onLoginReq(int linkid, Unpack* up) {
	PLoginRes res;
	UInfo* uinfo = NULL;

	PLoginReq req;
	req.unmarshall(*up);

	LOG(TAG_LOGIN, "login user with uid=%d, passport=%s.", req.uid, req.passport.c_str());
	//verify the user:
	//cache first:
	if( req.uid != 0 ) {
		uinfo = m_pLoginMgr->getUInfoMgr()->get(req.uid);
		if( uinfo == NULL ) {
			//uid !=0, user reconnect, but we have no info here in this proxy. That means user logon other proxy before.
			//need to notify the other proxy:
			LOG(TAG_LOGIN, "user logon other proxy before, uid=%d.", req.uid);
			uinfo = m_pLoginMgr->getMongo()->query(req.passport);
			if( uinfo == NULL ) {
				//uid!=0, mongo db empty, how?
				LOG(TAG_LOGIN, "wired, no login record for uid=%d", req.uid);
			} else {
				//notify that proxy, the user has logon another proxy.
				//[TBD]
				PMutiLoginNotify notify;	
				notify.old_router = uinfo->router;
				notify.old_dispatcher = uinfo->dispatcher;
				notify.old_proxy = uinfo->proxy;
				notify.new_router = m_pLoginMgr->getRouter();
				notify.new_dispatcher = m_pLoginMgr->getDispatcher();
				notify.new_proxy = m_pLoginMgr->getConfig()->name;

				Pack mpk(SVID_LOGIN, PMutiLoginNotify::uri);
				notify.marshall(mpk);
				mpk.pack();
				m_pLoginMgr->getLooper()->sendDispatcher( mpk.getBuf(), mpk.getLen() );
			}
		}
	} else {
		//mongodb next:
		uinfo = m_pLoginMgr->getMongo()->query(req.passport);
		if( uinfo != NULL ) {
			//add to cache, no matter passport is good or not.
			m_pLoginMgr->getUInfoMgr()->add(uinfo);
		}
	}
	if( uinfo == NULL ) {
		res.res = RES_FAIL;
	} else {
		if( uinfo->password != req.passport ) {
			res.res = RES_FAIL;
		} else {
			res.res = RES_OK;
			res.uid = uinfo->uid;
			res.cookie = uinfo->cookie;	//nothing for now.
		}
	}	
	Pack pk(SVID_LOGIN, PLoginRes::uri);
	res.marshall(pk);
	pk.pack();
	m_pLoginMgr->getLinkMgr()->send( linkid, pk.getBuf(), pk.getLen() );

	//save the uid to LinkMgr.
	m_pLoginMgr->getLinkMgr()->setUid(req.uid, linkid);
}

void	MsgHandler::onSendReq(int linkid, Unpack* up) {
	PSendReq req;
	req.unmarshall(*up);

	LOG(TAG_LOGIN, "send msg to from %d to peer %d, with seq=.", req.uid, req.peer, req.seq);
	//verify the uid is from the right link:
	if( !m_pLoginMgr->getLinkMgr()->check( req.uid, linkid) ) {
		LOG(TAG_LOGIN, "uid and linkid doesn't match, uid=%d, linkid=%d.", req.uid, linkid);
		return;
	}

	//send the request:
	if( m_pLoginMgr->getLinkMgr()->hasUid(req.peer) ) {
		//best scenario, uid/peer on the same proxy:
		m_pLoginMgr->getLinkMgr()->sendByUid(req.peer, up->getBuf(), up->getLen());
		return;
	} else {
		//send to dispatcher:
		m_pLoginMgr->getLooper()->sendDispatcher(up->getBuf(), up->getLen());
	}
}

