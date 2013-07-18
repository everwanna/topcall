#ifndef _PROXYMGR_H_
#define _PROXYMGR_H_

#include "config.h"
#include <map>
#include <string>

class MGMgr;
class ProxyInfo;
class ProxyMgr {
public:
	ProxyMgr();
	~ProxyMgr();

public:
	void	add(int linkid, const std::string& name, const std::string& ip, short port, bool udp, int load, int load_max);
	void	remove(int linkid);
	void	remove(const std::string& name);
	void	remove(const std::string& ip, short port);
	ProxyInfo*	getFreeProxy();
	void	getLoadInfo(int* load, int* load_max);	

	void	dump();

private:
	std::map<std::string, ProxyInfo*>	m_mapProxies;
	bool	m_bDirty;
};

#endif
