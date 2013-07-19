#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>
#include <string>

class NetLink;
class NetListener;
class DispMgr;
class NetLoop
{
public:
	NetLoop(DispMgr* mgr, const std::string& disp_ip, short disp_port, short port);
	~NetLoop();

public:
	void	run();		
	void	sendRouter(const char* data, int len);

	int		getSystemTime();
	DispMgr*getMgr() { return m_pMgr; }	
	char*	getBuffer() { return m_pBuffer; }
	event_base*	getEventBase() { return m_event_base; }

private:
	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	DispMgr*	m_pMgr;
	event_base*	m_event_base;

	NetLink*	m_pRouterLink;
	NetListener*m_pListener;
	char*		m_pBuffer;

	//timer:
	event*		m_pTimer;
};

#endif
