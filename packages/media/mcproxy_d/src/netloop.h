#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>
#include <string>

class MCPMgr;
class NetLoop
{
public:
	NetLoop(MCPMgr* mgr, const char* mc_ip, short mc_port, short port);
	~NetLoop();

public:
	void	run();
	void	send(int linkid, const char* msg, int len);	
	void	remove(int linkid);
	
	//mcenter link:
	void	connectMC(const char* mc_ip, short mc_port);
	void	sendMC(char *msg, int len);
	void	closeMC();
	void	startReconnMC();
	void	stopReconnMC();	

public:
	int		getSystemTime();

private:
	//tcp event handlers:
	static void tcp_listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void tcp_read_cb(struct bufferevent *, void *);
	static void tcp_write_cb(struct bufferevent *, void *);
	static void tcp_event_cb(struct bufferevent *, short, void *);

	//mcenter event handlers:	
	static void mc_read_cb(struct bufferevent *, void *);
	static void mc_write_cb(struct bufferevent *, void *);
	static void mc_event_cb(struct bufferevent *, short, void *);
	static void mc_reconn_cb(int fd, short event, void* arg);

	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	MCPMgr*		m_pMCPMgr;
	event_base*	m_event_base;
	evconnlistener *m_listener;
	
	short		m_nPort;
	char*		m_pBuffer;
	std::map<int, bufferevent*>	m_mapLinks;

	//mcenter link:
	bufferevent*m_pMCLink;
	std::string m_strMCIp;
	short		m_nMCPort;
	sockaddr_in m_addrMc;
	event*		m_pMCReconnTimer;

	//I need a timer:
	event*		m_pTimer;

	//hard code ping buf:
	Pack		m_pingPack;
};

#endif
