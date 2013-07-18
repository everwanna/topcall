#ifndef _MSGCACHE_H_
#define _MSGCACHE_H_

#include <map>

class Msg;

/**
 * Stream cache, each stream has a cache object.
 * The cache will holding about packets in latest 2 seconds, in case client want to resend a packet due to 
 * UDP packet lossless or other purpose
 *
 * The cache should be a ring buffer, new packets will replace old packets automatically.
 */
class MsgCache {
public:
	MsgCache(int max);
	~MsgCache();

public:
	void	add(int seq, Msg* msg);
	Msg*	find(int seq);

private:
	int		m_nMax;
	std::map<int, Msg*>	m_mapMsgs;
};

#endif
