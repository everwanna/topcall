#ifndef _ROUTERLINKMGR_H_
#define _ROUTERLINKMGR_H_

#include "config.h"
#include <map>
#include <string>

class RouterLinkMgr {
public:
	RouterLinkMgr();

public:
	void	set(const std::string& name, int linkid);
	void	remove(const std::string& name);
	int		get(const std::string& name);

private:
	std::map<std::string, int>	m_mapNameLinks;
};


#endif
