#include "pendlist.h"

PendList::PendList() {
}

PendList::~PendList() {
}

void	PendList::add(const std::string& strisp, int linkid) {
	std::map<std::string, std::set<int> >::iterator it = m_mapPendLinks.find(strisp);
	if( it == m_mapPendLinks.end() ) {
		std::set<int> links;
		links.insert(linkid);

		m_mapPendLinks[strisp] = links;
	} else {
		it->second.insert(linkid);
	}
}

void	PendList::remove(const std::string& strisp) {
	m_mapPendLinks.erase(strisp);
}

std::set<int>	PendList::get(const std::string& strisp) {
	std::map<std::string, std::set<int> >::iterator it = m_mapPendLinks.find(strisp);
	if( it == m_mapPendLinks.end() ) {
		return std::set<int>();
	} else {
		return it->second;
	}
}

void	PendList::clearExpire(int stamp) {

}

void	PendList::dump() {
	//LOG(TAG_MCPROXY, "PendList.dunp, size=%d", m_mapPendLinks.size());
	for( std::map<std::string, std::set<int> >::iterator it = m_mapPendLinks.begin(); it != m_mapPendLinks.end(); it++ ) {
		LOG(TAG_MCPROXY, "PendList.dunp, isp/size=%s, %d", it->first.c_str(), it->second.size());
	}
}
