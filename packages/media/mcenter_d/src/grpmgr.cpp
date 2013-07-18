#include "grpmgr.h"
#include "mcmgr.h"
#include "grpinfo.h"
#include "grpispmap.h"
#include "grpcache.h"
#include "netloop.h"

GrpMgr::GrpMgr(MCMgr* mgr) 
	: m_pMCMgr(mgr)
{
	m_pIspMap = new GrpIspMap();
	m_pCache = new GrpCache();
}

GrpMgr::~GrpMgr() {
	//clean the memory? not for now as the process is done when we come here.
	if( m_pIspMap ) {
		delete m_pIspMap;
	}
	if( m_pCache ) {
		delete m_pCache;
	}
}

void	GrpMgr::add(int linkid, const std::string& name, const std::string& ip, short port,  
					int isp, int area, int load, int load_max, 
					const std::string& access_url) {
	//add to global map first:
	GrpInfo* info = NULL;

	//add to name=>GrpInfo map:
	std::map<std::string, GrpInfo*>::iterator it = m_mapGroups.find(name);
	if( it != m_mapGroups.end() ) {
		info = it->second;
		if( info->getLoadMax() != load_max ) {
			LOG(TAG_MCENTER, "add group, name=%s, ip/port=%s:%d, isp=%d, area=%d, load=%d, load_max=%d.", name.c_str(), ip.c_str(), port,  isp, area, load, load_max);
		}

		info->setLoad(load);
		info->setLoadMax(load_max);
		info->setAccessUrl(access_url);
		info->setLastUpdate( m_pMCMgr->getLooper()->getSystemTime() );
	} else {
		info = new GrpInfo(name, ip, port);		
		info->setIsp(isp);
		info->setArea(area);
		info->setLoad(load);
		info->setLoadMax(load_max);
		info->setAccessUrl(access_url);
		info->setLastUpdate( m_pMCMgr->getLooper()->getSystemTime() );

		m_mapGroups[name] = info;
		LOG(TAG_MCENTER, "add group, name=%s, ip/port=%s:%d, isp=%d, area=%d, load=%d, load_max=%d, access_url=%s.", 
			name.c_str(), ip.c_str(), port,  isp, area, load, load_max, access_url.c_str());
	}

	//add to GrpIspMap:
	m_pIspMap->add(isp, area, info);

	//update the set:
	m_setIsps.insert(isp);
}

void	GrpMgr::remove(const std::string& name) {
	LOG(TAG_MCENTER, "remove group, name=%s.", name.c_str());

	GrpInfo* info = NULL;

	std::map<std::string, GrpInfo*>::iterator it = m_mapGroups.find(name);
	if( it != m_mapGroups.end() ) {
		info = it->second;
		m_mapGroups.erase(it);		
	}

	//remove from isp->group map:
	if( info != NULL ) {
		m_pIspMap->remove(info->getIsp(), info->getArea(), name);

		//delete the GrpInfo object:
		delete info;
	}
}

GrpInfo* GrpMgr::fetchGroup(int isp1, int area1, int isp2, int area2, int num) {	
	GrpInfo* info = NULL;

	//
	//[TBD] no cache policy for now, it's very difficult to make a good cache policy here for now.
	//Will test when we got at least 10K users.
	/*
	info = m_pCache->get(isp1, area1, isp2, area2);
	if( info != NULL ) {
		LOG(TAG_MCENTER, "GrpMgr::fetchGroup, hit cache for isp1/area1/isp2/area2=%d, %d, %d, %d", isp1, area1, isp2, area2);
		return info;
	}
	*/

	//fail to hit cache, do a deep search here:
	if( isp1 == isp2 ) {
		if( area1 == area2 ) {			
			info = m_pIspMap->findGroup(isp1, area1, num);
		} else {			
			info = m_pIspMap->findGroup(isp1, area1, num);
			if( info != NULL )
				goto exit;
			info = m_pIspMap->findGroup(isp1, area2, num);
		}
	} else {
		info = findDiffIsp(isp1, area1, isp2, area2, num);		
	}

	if( info != NULL )
		goto exit;

	info = findFallbackIsp(isp1, area1, isp2, area2, num);
	if( info != NULL )
		goto exit;

exit:
	//
	//[TBD] cache not enabled for now.
	//put the result into cache:
	/*
	if( info != NULL ) {
		m_pCache->put(isp1, area1, isp2, area2, info);
	}
	*/

#ifdef _DEBUG
	if( info != NULL ) {
		LOG(TAG_MCENTER, "fetch group, user1.isp/area=%d:%d, user2.isp/area=%d:%d, result=%s:%d:%d:%d:%d.", 
			isp1, area1, isp2, area2,
			info->getName().c_str(), info->getIsp(), info->getArea(), info->getLoad(), info->getLoadMax());
	}
#endif

	return info;
}

/**
 * A monitor system is in need.
 */
void	GrpMgr::removeExpire(int time) {
	GrpInfo* info = NULL;

	std::set<std::string> expires;
	for( std::map<std::string, GrpInfo*>::iterator it = m_mapGroups.begin(); it != m_mapGroups.end(); it++ ) {
		info = it->second;
		if( info->getLastUpdate() + GROUP_EXPIRE < time ) {
			//no update for too long time, we think the server is done...
			LOG(TAG_MCENTER, "remove expire group, name=%s.", info->getName().c_str());
			expires.insert(info->getName());
			
		}
	}

	for( std::set<std::string>::iterator it = expires.begin(); it != expires.end(); it++ ) {
		info = m_mapGroups[*it];
		m_mapGroups.erase(*it);
		m_pIspMap->remove( info->getIsp(), info->getArea(), info->getName() );
		delete info;
	}
}

void	GrpMgr::dump() {
	GrpInfo* info = NULL;

	LOG(TAG_MCENTER, "");
	LOG(TAG_MCENTER, "GrpMgr::dump =========");
	for( std::map<std::string, GrpInfo*>::iterator it = m_mapGroups.begin(); 
		it != m_mapGroups.end(); it++ ) {
		info = it->second;
		
		LOG(TAG_MCENTER, " name=%s, load=%d, load_max=%d", 
			info->getName().c_str(), 
			info->getLoad(),
			info->getLoadMax()
			);
	
	}
	LOG(TAG_MCENTER, "GrpMgr::dump =========");
	LOG(TAG_MCENTER, "");
}

GrpInfo*	GrpMgr::findDiffIsp(int isp1, int area1, int isp2, int area2, int num) {
	GrpInfo* info = NULL;
	int isp_all = isp1|isp2;

	info = m_pIspMap->findGroup(isp_all, area1, num);
	if( info != NULL )
		return info;
	info = m_pIspMap->findGroup(isp_all, area2, num);
	if( info != NULL )
		return info;

	//should go through all isps to see if any isp hold isp_all as a subset.
	return info;
}

/**
 * find fallback group list.
 *
 * [bugbug] here is a risk: if( isp_search, area1 ) doesn't exist, m_pIspMap->findGroup
 * will return (isp_search) result, findGroup(isp_search_area2) will never be hit.
 *
 */
GrpInfo*	GrpMgr::findFallbackIsp(int isp1, int area1, int isp2, int area2, int num) {
	GrpInfo* info = NULL;
	int isp_all = isp1|isp2;
	int isp_search = 0;

	for( std::set<int>::iterator it = m_setIsps.begin(); it != m_setIsps.end(); it++ ) {
		if( (isp_all&(*it)) == isp_all ) {
			isp_search = *it;
			
			info = m_pIspMap->findGroup(isp_search, area1, num);
			if( info )
				 return info;

			info = m_pIspMap->findGroup(isp_search, area2, num);
			if( info )
				return info;
		}
	}

	return NULL;
}
