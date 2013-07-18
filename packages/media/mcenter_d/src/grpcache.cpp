#include "grpcache.h"
#include "grpinfo.h"

GrpCache::GrpCache() {
}

GrpCache::~GrpCache() {
}

/**
 * There is a risk here, the load is not added when we return the GrpInfo as a usable group.
 * It should work for most of time, but if our resource is very limit, maybe problem here.
 * 
 * Should add a pre-alloc-load property later.
 *
 */
GrpInfo*	GrpCache::get(int isp1, int area1, int isp2, int area2) {
	char	strisp[64];

	sprintf(strisp, "%d:%d:%d:%d", isp1, area1, isp2, area2);
	std::map<std::string, GrpInfo*>::iterator it = m_mapCache.find(strisp);
	if( it == m_mapCache.end() ) 
		return NULL;

	GrpInfo* info = it->second;
	if( info->getLoadMax() - info->getLoad() < 100 ) {
		//almost use up, reporting
		m_mapCache.erase(it);
	}

	return info;
}

void	GrpCache::put(int isp1, int area1, int isp2, int area2, GrpInfo* group) {
	char	strisp[64];

	sprintf(strisp, "%d:%d:%d:%d", isp1, area1, isp2, area2);
	if( group->getLoadMax() - group->getLoad() < 100 ) {
		return;
	}

	m_mapCache[strisp] = group;
}

