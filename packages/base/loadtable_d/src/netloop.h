#ifndef NETLOOP_H
#define NETLOOP_H

#include "config.h"
#include <map>
#include <string>

class NetLink;
class NetListener;
class LTMgr;
class NetLoop
{
public:
	NetLoop(LTMgr* mgr, const std::string& master_ip, short master_port, short port);
	~NetLoop();

public:
	void	run();		
	void	sendMaster(const char* data, int len);

	int		getSystemTime();
	LTMgr*	getLTMgr() { return m_pMgr; }	
	char*	getBuffer() { return m_pBuffer; }
	event_base*	getEventBase() { return m_event_base; }

private:
	//timer event handler:
	static void timer_cb(int fd, short event, void* arg);

private:
	LTMgr*		m_pMgr;
	event_base*	m_event_base;

	NetLink*	m_pMasterLink;
	NetListener*m_pListener;
	char*		m_pBuffer;

	//timer:
	event*		m_pTimer;
};

#endif
