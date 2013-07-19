#include "routerlinkmgr.h"

RouterLinkMgr::RouterLinkMgr() {
}

void	RouterLinkMgr::set(const std::string& name, int linkid) {
	m_mapNameLinks[name] = linkid;
}

void	RouterLinkMgr::remove(const std::string& name) {
	m_mapNameLinks.erase(name);
}

int		RouterLinkMgr::get(const std::string& name) {
	std::map<std::string, int>::iterator it = m_mapNameLinks.find(name);
	if( it == m_mapNameLinks.end() ) {
		return -1;
	} else {
		return it->second;
	}
}

