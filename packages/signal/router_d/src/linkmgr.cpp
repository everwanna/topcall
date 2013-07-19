#include "linkmgr.h"
#include "link.h"
#include <vector>

LinkMgr::LinkMgr() {
}

void	LinkMgr::addLink(Link* link) {
	std::map<int, Link*>::iterator it = m_mapLinks.find(link->linkid);
	if( it == m_mapLinks.end() ) {
		m_mapLinks[ link->linkid] = link;
	} else {
		LOG(TAG_DISPATCHER, "LinkMgr::addLink, linkid exists, linkid=%d", link->linkid);
		return;
	}
}

void	LinkMgr::remove(int linkid) {
	std::map<int, Link*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) 
		return;

	Link* link = it->second;
	if( link ) {
		if( !link->closed ) {
			bufferevent_free(link->bev);
		}
		delete link;
	}
	m_mapLinks.erase(it);
}


void	LinkMgr::send(int linkid, const char* msg, int len) {
	std::map<int, Link*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		LOG(TAG_DISPATCHER, "LinkMgr::send, find linkid failed, linked=%d", linkid);
		return;
	}

	bufferevent_write(it->second->bev, msg, len);
}

void	LinkMgr::update(int linkid, int stamp) {
	std::map<int, Link*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		LOG(TAG_DISPATCHER, "LinkMgr::update, find linkid failed, linked=%d", linkid);
		return;
	}

	it->second->stamp = stamp;
}

void	LinkMgr::removeExpire(int time) {
	Link* link = NULL;
	std::vector<int> links;

	for(std::map<int, Link*>::iterator it = m_mapLinks.begin(); it != m_mapLinks.end(); it++ ) {
		link = it->second;

		if( link->stamp + LINK_EXPIRE < time ) {
			//it's expired:
			LOG(TAG_DISPATCHER, "LinkMgr::clearExpire, link expired, linkid/stamp/now=%d,%d,%d", link->linkid, link->stamp, time);
			links.push_back( link->linkid );
		}
	}

	//really remove the links now:
	for( size_t i=0; i<links.size(); i++ ) {
		remove( links[i] );

	}
}

void	LinkMgr::setUid(int uid, int linkid) {
	//verify the linkid exists:
	std::map<int, Link*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() )  {
		LOG(TAG_DISPATCHER, "linkid for uid doesn't exist, linkid=%d, uid=%d.", linkid, uid);
		return;
	}

	m_mapUidLinks[uid] = linkid;
	m_mapLinks[linkid]->uid = uid;
}

void	LinkMgr::sendByUid(int uid, const char* msg, int len) {
	std::map<int, int>::iterator it = m_mapUidLinks.find(uid);
	if( it == m_mapUidLinks.end() ) {
		LOG(TAG_DISPATCHER, "send with invalid uid=%d", uid);
		return;
	}

	send(it->second, msg, len);
}

bool	LinkMgr::hasUid(int uid) {
	return m_mapUidLinks.find(uid) != m_mapUidLinks.end();
}
