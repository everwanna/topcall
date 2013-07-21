#include "msglist.h"
#include "msg.h"
#include "loginmgr.h"
#include "linkmgr.h"

MsgList::MsgList(LoginMgr* mgr, int uid) 
	: m_pLoginMgr(mgr)
	, m_nUid(uid)
{
}

MsgList::~MsgList() {
}

void	MsgList::add(int seq, int uid, const std::string& data) {
	remove(seq);

	Msg* msg= new Msg();
	msg->uid = uid;
	msg->seq = seq;
	msg->payload = data;

	m_mapMsgs[seq] = msg;
}

void	MsgList::remove(int seq) {
	std::map<int, Msg*>::iterator it = m_mapMsgs.find(seq);
	if( it == m_mapMsgs.end() ) {
		return;
	}

	delete it->second;
	m_mapMsgs.erase(it);
}

Msg*	MsgList::get(int seq) {
	std::map<int, Msg*>::iterator it = m_mapMsgs.find(seq);
	if( it == m_mapMsgs.end() ) {
		return NULL;
	}

	return it->second;
}

void	MsgList::resend() {
	std::vector<int> removes;

	if( m_mapMsgs.size() == 0 )
		return;

	Msg* msg = NULL;
	for( std::map<int, Msg*>::iterator it=m_mapMsgs.begin(); it!=m_mapMsgs.end(); it++ ) {
		msg = it->second;
		m_pLoginMgr->getLinkMgr()->sendByUid( msg->uid, msg->payload.c_str(), msg->payload.length());
		msg->retry++;

		if( msg->retry > MSG_RESEND_MAX ) {
			removes.push_back(msg->seq);
		}
	}

	for( size_t i=0; i<removes.size(); i++ ) {
		LOG(TAG_LOGIN, " ERROR - fail to send msg, uid=%d, seq=%d, len=%d", msg->uid, msg->seq, msg->payload.length());
		m_mapMsgs.erase( removes[i] );
	}
}

