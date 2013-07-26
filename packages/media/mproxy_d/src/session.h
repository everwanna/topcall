#ifndef _SESSION_H_
#define _SESSION_H_

#include "config.h"
#include <map>

class SessMgr;
struct UInfo;
class Msg;
class Session {
public:
	Session(SessMgr* mgr, int sid);

public:
	void	onUserJoin(UInfo* uinfo);
	void	onUserLeave(int uid);
	void	onMsg(int linkid, int sid, int from, Msg* msg);

public:
	int		getSid() { return m_nSid; }
	void	getUsers(std::vector<UserMiniInfo> & users) const;

private:
	SessMgr*	m_pSessMgr;
	int			m_nSid;
	std::map<int, UInfo*>	m_mapUsers;

};

#endif
