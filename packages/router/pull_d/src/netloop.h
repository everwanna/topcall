#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>

class PullMgr;
class NetLoop
{
public:
	NetLoop(PullMgr* mgr, short port);
	~NetLoop();

public:
	void	run();
	void	send(int linkid, const char* msg, int len);

public:
	PullMgr*getMgr() { return m_pMgr; }

private:
	static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void read_cb(struct bufferevent *, void *);
	static void write_cb(struct bufferevent *, void *);
	static void event_cb(struct bufferevent *, short, void *);

	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	PullMgr*	m_pMgr;
	event_base*	m_event_base;
	evconnlistener *m_listener;
	short		m_port;
	char*		m_pBuffer;
	std::map<int, bufferevent*>	m_mapLinks;

	//timer:
	event*		m_pTimer;
};

#endif