#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>

class MDMgr;
class NetLoop
{
public:
	NetLoop(MDMgr* mgr, short port);
	~NetLoop();

public:
	void	run();
	void	send(int linkid, const char* msg, int len);
	void	send(int linkid, int uri, Packet& pack);
	void	remove(int linkid);

public:
	int		getSystemTime();

private:
	//tcp event handlers:
	static void tcp_listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void tcp_read_cb(struct bufferevent *, void *);
	static void tcp_write_cb(struct bufferevent *, void *);
	static void tcp_event_cb(struct bufferevent *, short, void *);

	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	MDMgr*		m_pMDMgr;
	event_base*	m_event_base;
	evconnlistener *m_listener;
	short		m_nPort;
	char*		m_pBuffer;
	std::map<int, bufferevent*>	m_mapLinks;

	//I need a timer:
	event*		m_pTimer;
};

#endif