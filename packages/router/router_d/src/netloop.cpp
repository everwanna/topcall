#include "netloop.h"
#include "routemgr.h"
#include "msghandler.h"
#include "linkmgr.h"
#include "link.h"
#include "netlink.h"
#include "netlistener.h"

NetLoop::NetLoop(RouteMgr* mgr, 
				 const std::string& dm_ip, short dm_port,
				 const std::string& push_ip, short push_port,
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

	//connect to deamon:
	{
		//m_pDMLink = new NetLink(this, "deamon", dm_ip, dm_port);
		//m_pDMLink->connect();
	}

	//connect to push_d
	{
		m_pPushLink = new NetLink(this, "push", push_ip, push_port);
		m_pPushLink->connect();
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
	//for test:
	/*
	login::PSendReq req;
	req.uid = 10000;
	req.peer = 10001;
	req.data = "good bye";

	Pack pk(SVID_LOGIN, login::PSendReq::uri);
	req.marshall(pk);
	pk.pack();
	m_pMgr->getHandler()->handle(0, pk.getBuf(), pk.getLen());
	*/
	while(true) {
		event_base_dispatch(m_event_base);
	}
}

void	NetLoop::sendPush(const char* data, int len) {
	if( m_pPushLink ) {
		m_pPushLink->send(data, len);
	} else {
		LOG(TAG_ROUTER, "send push, m_pPushLink==NULL.");
	}
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


