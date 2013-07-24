#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>

class PushMgr;
class NetLoop
{
public:
	NetLoop(PushMgr* mgr, short port);
	~NetLoop();

public:
	void	run();
	void	send(int linkid, const char* msg, int len);

private:
	static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void read_cb(struct bufferevent *, void *);
	static void write_cb(struct bufferevent *, void *);
	static void event_cb(struct bufferevent *, short, void *);

private:
	PushMgr*	m_pPushMgr;
	event_base*	m_event_base;
	evconnlistener *m_listener;
	short		m_port;
	char*		m_pBuffer;
	std::map<int, bufferevent*>	m_mapLinks;
};

#endif