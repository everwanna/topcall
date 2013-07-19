#ifndef _LINKMGR_H_
#define _LINKMGR_H_

#include "config.h"
#include <map>

struct Link;
class LinkMgr {
public:
	LinkMgr();

public:
	void	addLink(Link* link);
	void	remove(int linkid);
	void	send(int linkid, const char* msg, int len);
	void	update(int linkid, int stamp);
	void	removeExpire(int time);
	void	setUid(int uid, int linkid);
	void	sendByUid(int uid, const char* msg, int len);
	bool	hasUid(int uid);

private:
	//linkid=>Link.
	std::map<int, Link*>	m_mapLinks;
	std::map<int, int>		m_mapUidLinks;
};

#endif