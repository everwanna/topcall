#ifndef _NETLOOP_H_
#define _NETLOOP_H_

#include "config.h"
#include <string>

class MPMgr;
class NetLoop
{
public:
	NetLoop(MPMgr* mpmgr, const std::string& mg_ip, int mg_port, int port);
	~NetLoop();

public:
	void		run();
	void		send(int linkid, const sockaddr_in* addr, const char* msg, int len);
	int			getSystemTime();

	//mgroup:
	void		connectMG();
	void		sendMG(const char* msg, int len);
	void		closeMG();
	void		startReconnMG();
	void		stopReconnMG();

private:
	static void event_cb(int fd, short event, void* arg);
	static void read_cb(int fd, short event, void* arg);
	static void timer_cb(int fd, short event, void* arg);

	//mgroup:
	//tcp event handlers:	
	static void mgroup_read_cb(struct bufferevent *, void *);
	static void mgroup_write_cb(struct bufferevent *, void *);
	static void mgroup_event_cb(struct bufferevent *, short, void *);
	static void mgroup_reconn_cb(int fd, short event, void* arg);

private:
	MPMgr*		m_pMPMgr;
	event_base*	m_event_base;
	event*		m_timer;
	char*		m_pBuffer;	

	//mgroup link:
	bufferevent*m_pMGLink;
	std::string m_strMGIp;
	short		m_nMGPort;
	sockaddr_in m_addrMG;
	event*		m_pMGReconnTimer;
};

#endif
