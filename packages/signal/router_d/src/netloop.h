#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>
#include <string>

class NetLink;
class NetListener;
class RouteMgr;
class NetLoop
{
public:
	NetLoop(RouteMgr* mgr, 
			const std::string& router1_ip, short router1_port, 
			const std::string& router2_ip, short router2_port, 
			const std::string& router3_ip, short router3_port, 
			const std::string& router4_ip, short router4_port, 
			short port);
	~NetLoop();

public:
	void		run();

	int			getSystemTime();
	RouteMgr*	getMgr() { return m_pMgr; }	
	char*		getBuffer() { return m_pBuffer; }
	event_base*	getEventBase() { return m_event_base; }

private:
	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	RouteMgr*	m_pMgr;
	event_base*	m_event_base;
	NetListener*m_pListener;
	NetLink*	m_pRouter1;
	NetLink*	m_pRouter2;
	NetLink*	m_pRouter3;
	NetLink*	m_pRouter4;
	char*		m_pBuffer;

	//timer:
	event*		m_pTimer;
};

#endif
