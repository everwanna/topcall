#include "session.h"
#include "sessmgr.h"
#include "msg.h"
#include "uinfo.h"

Session::Session(SessMgr* mgr, int sid) 
	: m_pSessMgr(mgr)
	, m_nSid(sid)
{
}

void	Session::onUserJoin(UInfo* uinfo) {
	m_mapUsers[uinfo->uid] = uinfo;
}

void	Session::onUserLeave(int uid) {
	UInfo* uinfo = NULL;
	std::map<int, UInfo*>::iterator it = m_mapUsers.find(uid);
	if( it == m_mapUsers.end() ) {
		LOG(TAG_MPROXY, "Session::onUserLeave, user not found in session, uid/sid=%d, %d", uid, m_nSid);
		return;
	}
	uinfo = it->second;
	m_mapUsers.erase(it);
	delete uinfo;
}

void	Session::onMsg(int linkid, int sid, int from, Msg* msg) {
	std::map<int, UInfo*>::iterator it;
	UInfo* uinfo = NULL;

	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); it++ ) {
		uinfo = it->second;
		if( uinfo->uid == from ) {
			//echo back, not for now.
			//continue;
		}

		::sendto(linkid, msg->data(), msg->len(), 0, (const sockaddr*)&uinfo->addr, sizeof(uinfo->addr));
	}
}

void	Session::getUsers(std::vector<UserMiniInfo> & users) const {
	for (std::map<int, UInfo*>::const_iterator it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it) {
		UserMiniInfo user(it->first, it->second->nick);			
		users.push_back(user);
	}
}
