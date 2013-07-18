#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>

class MCMgr;
class NetLoop
{
public:
	NetLoop(MCMgr* mgr, const char* dm_ip, short dm_port, short port);
	~NetLoop();

public:
	void	run();	
	int		getSystemTime();

	void	connectDM();
	void	sendDM(const char* msg, int len);
	void	closeDM();
	void	startReconnDM();
	void	stopReconnDM();

private:
	//deamon handlers:	
	static void dm_read_cb(struct bufferevent *, void *);
	static void dm_write_cb(struct bufferevent *, void *);
	static void dm_event_cb(struct bufferevent *, short, void *);
	static void dm_reconn_cb(int fd, short event, void* arg);

	//tcp event handlers:
	static void tcp_listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void tcp_read_cb(struct bufferevent *, void *);
	static void tcp_write_cb(struct bufferevent *, void *);
	static void tcp_event_cb(struct bufferevent *, short, void *);

	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	MCMgr*		m_pMCMgr;
	event_base*	m_event_base;

	//deamon link:
	bufferevent*m_pDMLink;
	std::string m_strDMIp;
	short		m_nDMPort;
	sockaddr_in m_addrDM;
	event*		m_pDMReconnTimer;

	//listener:
	evconnlistener *m_listener;
	short		m_nPort;
	char*		m_pBuffer;	

	//I need a timer:
	event*		m_pTimer;
};

#endif
