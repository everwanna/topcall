#include "svcmgr.h"
#include "svclist.h"
#include "svcinfo.h"
#include "svcmgr.h"

SvcMgr::SvcMgr() {
}

SvcMgr::~SvcMgr() {
}

void	SvcMgr::add(const std::string& name, const std::string& ip, short port, short svid, int isp, int area, int load, int load_max, int stamp) {
	LOG(TAG_DEAMON, "add a svc, svid=%d, ip=%s, port=%d, isp=%d, area=%d", svid, ip.c_str(), port, isp, area);
	
	std::map<short, SvcList*>::iterator it = m_mapSvcs.find(svid);
	if( it == m_mapSvcs.end() ) {
		SvcList* list = new SvcList(svid);
		list->add(name, ip, port, svid, isp, area, load, load_max, stamp);
		m_mapSvcs[svid] = list;
	} else {
		m_mapSvcs[svid]->add(name, ip, port, svid, isp, area, load, load_max, stamp);
	}
}

SvcList*	SvcMgr::query(short svc) {
	std::map<short, SvcList*>::iterator it = m_mapSvcs.find(svc);
	if( it == m_mapSvcs.end() ) {
		return NULL;
	} else {
		return it->second;
	}
}

void	SvcMgr::removeExpire(int time) {
	for( std::map<short, SvcList*>::iterator it = m_mapSvcs.begin(); it != m_mapSvcs.end(); it++ ) {
		it->second->removeExpire(time);
	}
}

