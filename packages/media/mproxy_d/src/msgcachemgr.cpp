#include "msgcachemgr.h"
#include "msgcache.h"
#include "msg.h"

MsgCacheMgr::MsgCacheMgr() {
}

MsgCacheMgr::~MsgCacheMgr() {
}

void	MsgCacheMgr::addCache(int uid, int seq, Msg* msg) {
	if( m_mapCaches.find(uid) == m_mapCaches.end() )  {
		MsgCache* cache = new MsgCache(MSG_CACHE_SIZE);
		cache->add(seq, msg);

		m_mapCaches[uid] = cache;
	} else {
		m_mapCaches[uid]->add(seq, msg);
	}
}

Msg*	MsgCacheMgr::getCache(int uid, int seq) {
	if( m_mapCaches.find(uid) == m_mapCaches.end() ) {
		return NULL;
	}	

	return m_mapCaches[uid]->find(seq);
}

void	MsgCacheMgr::remove(int uid) {
	std::map<int, MsgCache*>::iterator it = m_mapCaches.find(uid);

	if( it == m_mapCaches.end() ) {
		return;
	}
	
	MsgCache* cache = it->second;
	delete cache;
	m_mapCaches.erase(it);
}
