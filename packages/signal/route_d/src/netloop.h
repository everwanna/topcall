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
	NetLoop(RouteMgr* mgr, const std::string& disp_ip, short disp_port, short port);
	~NetLoop();

public:
	void	run();		
	void	sendDispatcher(const char* data, int len);

	int		getSystemTime();
	RouteMgr*	getLTMgr() { return m_pMgr; }	
	char*	getBuffer() { return m_pBuffer; }
	event_base*	getEventBase() { return m_event_base; }

private:
	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	RouteMgr*	m_pMgr;
	event_base*	m_event_base;

	NetLink*	m_pDispLink;
	NetListener*m_pListener;
	char*		m_pBuffer;

	//timer:
	event*		m_pTimer;
};

#endif
