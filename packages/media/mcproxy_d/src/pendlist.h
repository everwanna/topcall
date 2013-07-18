#ifndef _PENDLIST_H_
#define _PENDLIST_H_

#include "config.h"
#include <map>
#include <set>
#include <string>

class PendItem;
class PendList {
public:
	PendList();
	~PendList();

public:
	void	add(const std::string& strisp, int linkid);
	void	remove(const std::string& strisp);
	std::set<int>	get(const std::string& strisp);
	void	clearExpire(int stamp);
	void	dump();

private:
	std::map<std::string, std::set<int> > m_mapPendLinks;
};

#endif
