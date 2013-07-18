#ifndef _CACHE_H_
#define _CACHE_H_

#include "config.h"
#include <map>
#include <list>
#include <string>

class MCPMgr;
struct CacheItem;
class CacheList;
class Cache {
public:
	Cache(MCPMgr* mgr);
	~Cache();

public:
	void		add(const std::string& isp, int stamp, int num, const std::string& msg);
	std::string	get(const std::string& isp);
	int			getSize(const std::string& isp);
	void		remove(const std::string& isp);
	void		clearExpire(int stamp);
	void		dump();

private:
	MCPMgr*		m_pMCPMgr;
	std::map<std::string, CacheList*>	m_mapCache;
};

struct CacheItem {
	int		stamp;
	int		remain;
	std::string	msg;
};

class CacheList {
public:
	CacheList();
	~CacheList();

public:
	void		add(int stamp, int num, const std::string& msg);
	CacheItem*	get();
	int			size();
	void		clearExpire(int stamp);

private:
	std::list<CacheItem*>	m_arrItems;
};

#endif
