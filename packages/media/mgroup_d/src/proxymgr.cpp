#include "proxymgr.h"
#include "mgmgr.h"
#include "proxyinfo.h"

ProxyMgr::ProxyMgr() 
	: m_bDirty(false)
{
}

ProxyMgr::~ProxyMgr() {
	//clean the memory? not for now as the process is done when we come here.
}

void	ProxyMgr::add(int linkid, const std::string& name, const std::string& ip, short port, bool udp, int load, int load_max) {
	//add to global map first:
	ProxyInfo* info = NULL;

	std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.find(name);
	if( it != m_mapProxies.end() ) {
		//hack here, only compare the load field:		
		info = it->second;

		if( load == info->getLoad() && load_max == info->getLoadMax() ) {
			//do nothing here.
		} else {
			info->setLoad(load);
			info->setLoadMax(load_max);
			m_bDirty = true;
		}
	} else {		
		info = new ProxyInfo(linkid, name, ip, port, udp);		
		info->setLoad(load);
		info->setLoadMax(load_max);

		m_mapProxies[name] = info;
		m_bDirty = true;

		LOG(TAG_MGROUP, "ProxyMgr::add, linkid=%d, name=%s, ip/port/load/load_max=%s, %d, %d, %d", linkid, name.c_str(), ip.c_str(), port, load, load_max);
	}
}

void	ProxyMgr::remove(int linkid) {
	LOG(TAG_MGROUP, "ProxyMgr::remove, linkid=%d.", linkid);

	ProxyInfo* info = NULL;
	for( std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.begin(); 
		it != m_mapProxies.end(); it++ ) {
		info = it->second;
		if( info->getLinkId() == linkid ) {
			m_mapProxies.erase(it);
			delete info;
			break;
		}
	}
	m_bDirty = true;
}

void	ProxyMgr::remove(const std::string& name) {
	LOG(TAG_MGROUP, "remove proxy, name=%s.", name.c_str());

	ProxyInfo* info = NULL;

	std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.find(name);
	if( it != m_mapProxies.end() ) {
		info = it->second;
		m_mapProxies.erase(it);
		delete info;
	}

	m_bDirty = true;
}

void	ProxyMgr::remove(const std::string& ip, short port) {
	LOG(TAG_MGROUP, "remove proxy, ip/port=%s, %d.", ip.c_str(), port);

	ProxyInfo* info = NULL;
	std::string name = ip;
	char cname[12];

	sprintf(cname, ":%d", port);
	name = ip + cname;

	std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.find(name);
	if( it != m_mapProxies.end() ) {
		info = it->second;
		m_mapProxies.erase(it);
		delete info;
	}

	m_bDirty = true;
}

ProxyInfo*	ProxyMgr::getFreeProxy() {
	std::map<std::string, ProxyInfo*>::iterator it;
	ProxyInfo* info = NULL;
	int score = 0;
	int score_tmp = 0;

	for( it = m_mapProxies.begin(); it != m_mapProxies.end(); it++ ) {
		info = it->second;

		score_tmp = info->getLoad()*100/info->getLoadMax();
		if( score_tmp > score ) {
			break;
		}
	}

	return info;
}

void	ProxyMgr::getLoadInfo(int* load, int* load_max) {	
	*load = 0;
	*load_max = 0;

	for( std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.begin(); it != m_mapProxies.end(); it++ ) {
		*load += it->second->getLoad();	
		*load_max += it->second->getLoadMax();
	}
}

void	ProxyMgr::dump() {
	ProxyInfo* info = NULL;

	if( !m_bDirty ) {
		//nothing to dump.
		return;
	}

	LOG(TAG_MGROUP, "");
	LOG(TAG_MGROUP, "ProxyMgr::dump =========");
	for( std::map<std::string, ProxyInfo*>::iterator it = m_mapProxies.begin(); 
		it != m_mapProxies.end(); it++ ) {
		info = it->second;
		
		LOG(TAG_MGROUP, " name=%s, ip=%s, port=%d, udp=%d, load=%d, load_max=%d", 
			info->getName().c_str(), 
			info->getIp().c_str(), 
			info->getPort(),
			info->getUdp(),
			info->getLoad(),
			info->getLoadMax()
			);
	
	}
	LOG(TAG_MGROUP, "ProxyMgr::dump =========");
	LOG(TAG_MGROUP, "");

	m_bDirty = false;
}
