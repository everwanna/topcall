#include "msgpool.h"
#include "msg.h"

MsgPool::MsgPool() {
	for( int i=0; i<MSG_POOL_SIZE; i++ ) {
		Msg* msg = new Msg();
		m_arrMsgs.push_back(msg);
	}
}

MsgPool::~MsgPool() {
	//do I really need to delete them?
	//The process is down when I comes here.
}

Msg*	MsgPool::fetch() {
	Msg* msg = NULL;

	if( m_arrMsgs.size() != 0 ) {
		msg = *m_arrMsgs.begin();
		m_arrMsgs.pop_front();
	} else {
		msg = new Msg();
	}

	return msg;
}

void	MsgPool::free(Msg* msg) {
	if( m_arrMsgs.size() > MSG_POOL_SIZE ) {
		delete msg;
		return;
	} else {
		m_arrMsgs.push_back(msg);
	}
}
