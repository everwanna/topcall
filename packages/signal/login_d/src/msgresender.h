#ifndef _RESEND_H_
#define _RESEND_H_

#include "config.h"
#include <map>
#include <string>

class LoginMgr;
class MsgList;
class MsgResender {
public:
	MsgResender(LoginMgr* mgr);
	~MsgResender();

public:
	void	add(int uid, int seq, const std::string& payload);
	void	onTimer();

private:
	LoginMgr*	m_pLoginMgr;
	std::map<int, MsgList*>	m_mapUidMsgs;
};

#endif
