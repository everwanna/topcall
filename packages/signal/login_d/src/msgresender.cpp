#include "msgresender.h"
#include "msglist.h"
#include "msg.h"

MsgResender::MsgResender(LoginMgr* mgr) 
	: m_pLoginMgr(mgr)
{
}

void	MsgResender::add(int uid, int seq, const std::string& payload) {
	std::map<int, MsgList*>::iterator it = m_mapUidMsgs.find(uid);
	MsgList* mlist = NULL;

	if( it == m_mapUidMsgs.end() )  {
		mlist = new MsgList(m_pLoginMgr, uid);
	} else {
		mlist = it->second;
	}

	mlist->add(seq, uid, payload);
}

/**
 * resend the pkgs which is sent failed.
 * 
 */
void	MsgResender::onTimer() {
	for( std::map<int, MsgList*>::iterator it = m_mapUidMsgs.begin(); it != m_mapUidMsgs.end(); it++ ) {
		it->second->resend();
	}
}

