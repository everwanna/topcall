#include "slavemgr.h"
#include "slave.h"
#include "ltmgr.h"
#include "linkmgr.h"

SlaveMgr::SlaveMgr(LTMgr* mgr) 
	: m_pLTMgr(mgr)
{
}

SlaveMgr::~SlaveMgr() {
}

void	SlaveMgr::add(int linkid, const std::string& name, int time) {
	Slave* slave = NULL;
	std::map<int,Slave*>::iterator it = m_mapSlaves.find(linkid);
	if( it == m_mapSlaves.end() ) {
		slave = new Slave();
		slave->linkid = linkid;
		slave->name = name;
		slave->stamp = time;

		m_mapSlaves[linkid] = slave;
	} else {
		m_mapSlaves[linkid]->name = name;
	}
}

void	SlaveMgr::remove(int linkid) {
	std::map<int,Slave*>::iterator it = m_mapSlaves.find(linkid);
	if( it != m_mapSlaves.end() ) {
		m_mapSlaves.erase(it);
	}
}

void	SlaveMgr::send(const char* data, int len) {
	for( std::map<int,Slave*>::iterator it = m_mapSlaves.begin(); it != m_mapSlaves.end(); it++ ) {
		m_pLTMgr->getLinkMgr()->send( it->first, data, len);
	}
}

void	SlaveMgr::update(int linkid, int time) {
	std::map<int,Slave*>::iterator it = m_mapSlaves.find(linkid);
	if( it != m_mapSlaves.end() ) {
		it->second->stamp = time;
	}
}

void	SlaveMgr::removeExpired(int time) {
	Slave* slave = NULL;
	for( std::map<int,Slave*>::iterator it = m_mapSlaves.begin(); it != m_mapSlaves.end(); it++ ) {
		if( it->second->stamp + LINK_EXPIRE <= time ) {
			slave = it->second;
			m_mapSlaves.erase(it);
			m_pLTMgr->getLinkMgr()->remove(it->first);

			delete slave;
			return;
		}
	}
}
