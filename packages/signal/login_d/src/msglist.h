#ifndef _MSGLIST_H_
#define _MSGLIST_H_

#include <map>
#include <string>

class LoginMgr;
struct Msg;
class MsgList {
public:
	MsgList(LoginMgr* mgr, int uid);
	~MsgList();

public:
	void	add(int seq, int uid, const std::string& data);
	void	remove(int seq);
	Msg*	get(int seq);
	void	resend();

private:
	LoginMgr*	m_pLoginMgr;
	int			m_nUid;
	std::map<int, Msg*>	m_mapMsgs;
};

#endif
