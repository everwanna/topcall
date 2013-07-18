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
		LOG(TAG_LTABLE, "LinkMgr::addLink, linkid exists, linkid=%d", link->linkid);
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
		LOG(TAG_LTABLE, "LinkMgr::send, find linkid failed, linked=%d", linkid);
		return;
	}

	bufferevent_write(it->second->bev, msg, len);
}

void	LinkMgr::update(int linkid, int stamp) {
	std::map<int, Link*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		LOG(TAG_LTABLE, "LinkMgr::update, find linkid failed, linked=%d", linkid);
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
			LOG(TAG_LTABLE, "LinkMgr::clearExpire, link expired, linkid/stamp/now=%d,%d,%d", link->linkid, link->stamp, time);
			links.push_back( link->linkid );
		}
	}

	//really remove the links now:
	for( size_t i=0; i<links.size(); i++ ) {
		remove( links[i] );
	}
}
