#include "cache.h"
#include "mcpmgr.h"
#include "netloop.h"

Cache::Cache(MCPMgr* mgr) : m_pMCPMgr(mgr)
{
}

Cache::~Cache() {
	//the process is down, not need to release.
	CacheList* cl = NULL;
	for( std::map<std::string, CacheList*>::iterator it = m_mapCache.begin(); it != m_mapCache.end(); it++ ) {
		cl = it->second;
		if( cl != NULL ) {
			delete cl;
		}
	}

	m_mapCache.clear();
}

void	Cache::add(const std::string& isp, int stamp, int num, const std::string& msg) {
	CacheList* list = NULL;	

	std::map<std::string, CacheList*>::iterator it = m_mapCache.find(isp);
	if( it != m_mapCache.end() ) {
		list = it->second;
		list->add(stamp, num, msg);
	} else {
		list = new CacheList();
		list->add(stamp, num, msg);

		m_mapCache[isp] = list;
	}
}

std::string		Cache::get(const std::string& isp) {
	std::map<std::string, CacheList*>::iterator it = m_mapCache.find(isp);
	if( it == m_mapCache.end() ) 
		return "";

	CacheItem* item = it->second->get();
	if( item == NULL )
		return "";

	return item->msg;
}

int			Cache::getSize(const std::string& isp) {
	std::map<std::string, CacheList*>::iterator it = m_mapCache.find(isp);
	if( it == m_mapCache.end() ) 
		return 0;

	return it->second->size();
}

void	Cache::remove(const std::string& isp) {
	std::map<std::string, CacheList*>::iterator it = m_mapCache.find(isp);
	if( it == m_mapCache.end() ) 
		return;

	CacheList* cl = it->second;
	if( cl != NULL ) {
		delete cl;
	}

	//remove the specified isp map.
	m_mapCache.erase(it);	
}

void	Cache::clearExpire(int stamp) {
	for( std::map<std::string, CacheList*>::iterator it = m_mapCache.begin(); it != m_mapCache.end(); it++ ) {
		it->second->clearExpire(stamp);
	}
}

void	Cache::dump() {
	for( std::map<std::string, CacheList*>::iterator it = m_mapCache.begin(); it != m_mapCache.end(); it++ ) {
		LOG(TAG_MCPROXY, "Cache::dump, isp/num=%s, %d", it->first.c_str(), it->second->size());
	}
}


CacheList::CacheList() {
}

CacheList::~CacheList() {
	for( std::list<CacheItem*>::iterator it = m_arrItems.begin(); it != m_arrItems.end(); it++ ) {
		CacheItem* item = *it;
		if( item ) {
			delete item;
		}
	}

	m_arrItems.clear();
}

void	CacheList::add(int stamp, int num, const std::string& msg) {
	CacheItem* item = new CacheItem();
	item->stamp = stamp;
	item->remain = num;
	item->msg = msg;

	m_arrItems.push_back(item);
}

CacheItem*	CacheList::get() {
	CacheItem* item = NULL;
	std::list<CacheItem*>::iterator it = m_arrItems.begin();

	//[TBD] double check no iterator corrupt here:
	while(it != m_arrItems.end() ) {
		item = *it;
		if( item == NULL ) {
			LOG(TAG_MCPROXY, "CacheList::get, find item == NULL, critical issue.");
			m_arrItems.erase(it);
			it++;
			continue;
		}

		if( item->remain <= 0 ) {
			LOG(TAG_MCPROXY, "CacheList::get, find item.remain==0, critical issue.");
			m_arrItems.erase(it);
			it++;
			continue;
		}

		item->remain--;
		if( item->remain <= 0 ) {
			m_arrItems.erase(it);
		}

		break;
	}

	return item;
}

int		CacheList::size() {
	int count = 0;
	for( std::list<CacheItem*>::iterator it = m_arrItems.begin(); it != m_arrItems.end(); it++ ) {
		count += (*it)->remain;
	}

	return count;
}


void	CacheList::clearExpire(int stamp) {	
	for( std::list<CacheItem*>::iterator it = m_arrItems.begin(); it != m_arrItems.end(); it++ ) {
		if( (*it)->stamp + CACHE_EXPIRE_INTERVAL < stamp ) {
			LOG(TAG_MCPROXY, "ClearList::clearExpire, clear cache num/stamp=%d, %d.", (*it)->remain, (*it)->stamp);
			m_arrItems.erase(it);
			break;	//remove one cache each time;
		}
	}
}
