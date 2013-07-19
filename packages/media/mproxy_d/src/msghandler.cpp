#include "msghandler.h"
#include "mpmgr.h"
#include "sessmgr.h"
#include "session.h"
#include "uinfo.h"
#include "msgpool.h"
#include "msg.h"
#include "msgcachemgr.h"
#include "loadmgr.h"
#include "netloop.h"

MsgHandler::MsgHandler(MPMgr* mgr) {
	m_pMPMgr = mgr;
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, const sockaddr_in* peer_addr, char* msg, int len) {
#ifdef DEBUG
	//LOG(TAG_MPROXY, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	Unpack up(msg, len);
	up.popHead();
	
	if( up.getSvid() != SVID_MPROXY && up.getSvid() != SVID_MGROUP ) {
		LOG(TAG_MPROXY, "MsgHandler::handle, svid!=SVID_MPROXY|SVID_MGROUP, svid=%d", up.getSvid());
		//[TBD]
		//Should consider the blocklist.
		return;
	}

	//
	//special case here, because most of the packages are PMPStreamData, I don't want the switch jump.
	//Audio PPS: 5000*25 = 125k (package per second)
	//
	if( up.getUri() == PMPStreamData::uri ) {
		onMPStreamData(linkid, peer_addr, &up);
		return;
	}

	switch(up.getUri() ) {
	case PMPCreateReq::uri:
		onCreateReq(linkid, peer_addr, &up);
		break;
	case PMPJoinReq::uri:
		onMPJoinReq(linkid, peer_addr, &up);
		break;
	case PMPPing::uri:
		onMPPing(linkid, peer_addr, &up);
		break;
	case PMPResendReq::uri:
		onMPResendReq(linkid, peer_addr, &up);
		break;
	default:
		LOG(TAG_MPROXY, "MsgHandler::handle, unknown package, svid=%d, uri=%d", up.getSvid(), up.getUri());
		//[TBD]
		//should consider the blacklist.
		break;
	}
}

void	MsgHandler::onCreateReq(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	PMPCreateRes res;

	PMPCreateReq req;
	req.unmarshall(*up);

	LOG(TAG_MPROXY, "MsgHandler::onCreateReq, linkid=%d, uid=%d, stream=%s", linkid, req.uid, req.stream.c_str());
	Session* sess = m_pMPMgr->getSessMgr()->createSession(req.stream);
	if( sess == NULL ) {
		res.res = RES_FAIL;
	} else {
		res.res = RES_OK;
		res.uid = req.uid;
		res.nick = req.nick;
		res.stream = req.stream;		
	}
	Pack pk(SVID_MPROXY, PMPCreateRes::uri);
	res.marshall(pk);
	pk.pack();
	m_pMPMgr->getLooper()->send(linkid, peer_addr, pk.getBuf(), pk.getLen() );
}

void	MsgHandler::onMPJoinReq(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	LOG(TAG_MPROXY, "MsgHandler::onMPJoinReq");

	UInfo* uinfo = NULL;
	Session* session = NULL;

	int rc = RES_OK;
	PMPJoinRes res;

	PMPJoinReq req;
	req.unmarshall(*up);

	if( !m_pMPMgr->getLoadMgr()->hasLoad() ) {
		rc = RES_OVERLOAD;
		goto exit;
	} 	

	session = m_pMPMgr->getSessMgr()->getSession(req.sid);
	if( session == NULL ) {
		LOG(TAG_MPROXY, "MsgHandler::onMPJoinReq, session not found for sid=%d, stream=%s.", req.sid, req.stream.c_str());
		rc = RES_NO_GROUP;
		goto exit;
	}
	uinfo = new UInfo();
	uinfo->uid = req.uid;
	uinfo->addr = *peer_addr;
	uinfo->nick = req.nick;
	session->onUserJoin(uinfo);

	res.res = rc;	
	res.sid = session->getSid();
	res.uid = req.uid;	

	LOG(TAG_MPROXY, "MsgHandler::onMPJoinReq, res/sid/uid/nick=%d, %d, %d, %s", rc, res.sid, res.uid, req.nick.c_str());

exit:
	Pack pk(SVID_MPROXY, PMPJoinRes::uri);
	res.marshall(pk);
	pk.pack();
	
	m_pMPMgr->getLooper()->send(linkid, peer_addr, pk.getBuf(), pk.getLen());
}

void	MsgHandler::onMPLeaveReq(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	LOG(TAG_MPROXY, "MsgHandler::onMPUnsubscribeReq");

	PMPLeaveReq req;
	req.unmarshall(*up);
	
	//[TBD] should validate the uid.
	
}

void	MsgHandler::onMPPing(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	PMPPing ping;
	ping.unmarshall(*up);
	
	
}

void	MsgHandler::onMPStreamData(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	//some hack here, don't really unpack the full msg:
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_MPROXY, "MsgHandler::onMPStreamData, seq=%d.", seq++);
#endif
	//dispatch the stream data:
	Msg* msg = m_pMPMgr->getMsgPool()->fetch();
	if( msg == NULL ) {
		LOG(TAG_MPROXY, "MsgHandler::onMPStreamData, fetch msg from msg pool failed");
		return;
	}
	msg->assign(up->getBuf(), up->getLen() );

	int sid = up->popInt32();
	int from = up->popInt32();
	m_pMPMgr->push(linkid, sid, from, msg);
}

/**
 * If the client request resend packet, should find the packet in cache and return the data with PMPStreamData.
 *
 * @linkid the linkid(fd) of the request.
 * @peer_addr peer udp address.
 * @up Unpack object, with uri/svid info.
 */
void	MsgHandler::onMPResendReq(int linkid, const sockaddr_in* peer_addr, Unpack* up) {
	PMPStreamData data;
	data.unmarshall(*up);

	Msg* msg =  m_pMPMgr->getCacheMgr()->getCache(data.from, data.seq);
	if( msg != NULL ) {
		PMPStreamData sd;
		sd.from = data.from;
		sd.seq = data.seq;
		sd.payload = std::string(msg->data(), msg->len());

		Pack pk(SVID_MPROXY, PMPStreamData::uri);
		sd.marshall(pk);
		pk.pack();
	
		m_pMPMgr->getLooper()->send(linkid, peer_addr, pk.getBuf(), pk.getLen());
	}
}
