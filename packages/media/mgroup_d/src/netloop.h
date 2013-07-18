#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>
#include <string>

class NetLink;
class MGMgr;
class NetLoop
{
public:
	NetLoop(MGMgr* mgr, const std::string& mc_ip, short mc_port, const std::string& lt_ip, short lt_port, short udp_port);
	~NetLoop();

public:
	void	run();	
	void	sendMC(const char* data, int len);
	void	sendLT(const char* data, int len);

	int		getSystemTime();
	MGMgr*	getMGMgr() { return m_pMGMgr; }
	event_base*	getEventBase() { return m_event_base; }
	char*	getBuffer() { return m_pBuffer; }

private:
	//mproxy event handlers:
	static void mp_listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void mp_read_cb(struct bufferevent *, void *);
	static void mp_write_cb(struct bufferevent *, void *);
	static void mp_event_cb(struct bufferevent *, short, void *);

	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	MGMgr*		m_pMGMgr;
	event_base*	m_event_base;

	NetLink*	m_pMCLink;
	NetLink*	m_pLTLink;

	//tcp port to monitor mproxy	
	evconnlistener *m_listener;
	short		m_nMPPort;	
	char*		m_pBuffer;

	//timer:
	event*		m_pTimer;
};

#endif
