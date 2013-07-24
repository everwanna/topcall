#include "msghandler.h"
#include "pushmgr.h"

MsgHandler::MsgHandler(PushMgr* mgr) {
	m_pPushMgr = mgr;
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
	if( msg == NULL || len <= 4 ) {
		return;
	}

	Unpack up(msg, len);
	up.popHead();
	
	switch(up.getUri() ) {
	case PConsumerRegisterReq::uri:
		onConsumerRegisterReq(linkid, &up);
		break;
	case PPushMsg::uri:
		onPushMsg(linkid, &up);
		break;
	}
}

void	MsgHandler::onConsumerRegisterReq(int linkid, Unpack* up) {
	LOG(TAG_PUSH, "MsgHandler::onConsumerRegisterReq.");

	PConsumerRegisterReq req;
	req.unmarshall(*up);

	m_pPushMgr->registerConsumer(linkid, req.name, req.topic);
}

void	MsgHandler::onPushMsg(int linkid, Unpack* up) {
	//some hack here, don't really unpack the full msg:
	//static int seq = 0;
	//LOG(TAG_PUSH, "MsgHandler::onPushMsg, seq=%d.", seq++);

	PPushMsg msg;
	msg.unmarshall(*up);

	m_pPushMgr->push(msg.topic, up->getBuf(), up->getLen() );
}