#include "netloop.h"
#include "routemgr.h"
#include "msghandler.h"
#include "linkmgr.h"
#include "link.h"
#include "netlink.h"
#include "netlistener.h"

NetLoop::NetLoop(RouteMgr* mgr, 
				 const std::string& router1_ip, short router1_port, 
				 const std::string& router2_ip, short router2_port, 
				 const std::string& router3_ip, short router3_port, 
				 const std::string& router4_ip, short router4_port, 
				 short port) 
	: m_pMgr(mgr)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_ROUTER, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//connect to other routers:
	{

		if( router1_ip.length() != 0 && router1_port != 0 ) {
			m_pRouter1 = new NetLink(this, "router1", router1_ip.c_str(), router1_port);
			m_pRouter1->connect();
		}
		if( router2_ip.length() != 0 && router2_port != 0 ) {
			m_pRouter2 = new NetLink(this, "router2", router2_ip.c_str(), router2_port);
			m_pRouter2->connect();
		}
		if( router3_ip.length() != 0 && router3_port != 0 ) {
			m_pRouter3 = new NetLink(this, "router3", router3_ip.c_str(), router3_port);
			m_pRouter3->connect();
		}
		if( router4_ip.length() != 0 && router4_port != 0 ) {
			m_pRouter4 = new NetLink(this, "router4", router4_ip.c_str(), router4_port);
			m_pRouter4->connect();
		}
	}

	//startup tcp listener:
	m_pListener = new NetListener(this);
	m_pListener->listen(port);

	//init the buffer:
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//start a timer to do scheduler job:
	{
		timeval tm;
		tm.tv_sec = TIMER_INTERVAL;
		tm.tv_usec = 0;
		m_pTimer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
		evtimer_add(m_pTimer, &tm);
	}
}

NetLoop::~NetLoop()
{
	if( m_event_base )
	{
		event_base_free(m_event_base);
		m_event_base = NULL;
	}
	if( m_pBuffer ) {
		delete[] m_pBuffer;
	}
}

void	NetLoop::run() 
{
	event_base_dispatch(m_event_base);
}

/**
 * This function must be optimize later.
 */
int		NetLoop::getSystemTime() {
	timeval tv;
	evutil_gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMgr->onTimer();
}


