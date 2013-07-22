#ifndef _NETLINK_H_
#define _NETLINK_H_

#include "config.h"
#include "netloop.h"

class NetLink {
public:
	NetLink(NetLoop* loop, const std::string& name, const std::string& ip, short port);
	~NetLink();

public:
	void	connect();
	int		send(const char* msg, int len);
	void	close();
	void	startReconn();
	void	stopReconn();

private:	
	static void read_cb(struct bufferevent *, void *);
	static void write_cb(struct bufferevent *, void *);
	static void event_cb(struct bufferevent *, short, void *);
	static void reconn_cb(int fd, short event, void* arg);

private:
	std::string	m_strName;
	NetLoop*	m_pLooper;
	bufferevent*m_pEvent;
	std::string m_strIp;
	short		m_nPort;
	sockaddr_in m_addr;
	event*		m_pReconnTimer;
};

#endif
