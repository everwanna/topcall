#ifndef _LINKMGR_H_
#define _LINKMGR_H_

#include "config.h"
#include <map>

class LoginMgr;
struct Link;
class LinkMgr {
public:
	LinkMgr(LoginMgr* mgr);

public:
	void	addLink(Link* link);
	void	remove(int linkid);
	void	send(int linkid, const char* msg, int len);
	void	update(int linkid, int stamp);
	void	removeExpire(int time);
	void	setUid(int uid, int linkid);
	void	sendByUid(int uid, const char* msg, int len);
	bool	hasUid(int uid);
	bool	check(int uid, int linkid);

private:
	//linkid=>Link.
	LoginMgr*	m_pLoginMgr;
	std::map<int, Link*>	m_mapLinks;
	std::map<int, int>		m_mapUidLinks;
};

#endif