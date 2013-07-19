#ifndef _NETLISTENER_H_
#define _NETLISTENER_H_

#include "config.h"

class NetLoop;
class NetListener {
public:
	NetListener(NetLoop* loop);
	~NetListener();

public:
	void	listen(short port);

public:
	static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void read_cb(struct bufferevent *, void *);
	static void write_cb(struct bufferevent *, void *);
	static void event_cb(struct bufferevent *, short, void *);

private:
	NetLoop*		m_pLooper;
	evconnlistener *m_listener;	
};

#endif
