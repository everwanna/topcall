#include "netloop.h"
#include "mdmgr.h"
#include "msghandler.h"

NetLoop::NetLoop(MDMgr* mgr, short port) 
	: m_pMDMgr(mgr)
	, m_nPort(port)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_DEAMON, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//startup tcp listener:
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	m_listener = evconnlistener_new_bind(m_event_base, tcp_listener_cb, (void *)this,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	//init the buffer:
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//init the timer:
	timeval tm;
	tm.tv_sec = TIMER_INTERVAL;
	tm.tv_usec = 0;
	m_pTimer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
    evtimer_add(m_pTimer, &tm);
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

void	NetLoop::send(int linkid, const char* msg, int len)
{
	std::map<int, bufferevent*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		return;
	}

	bufferevent_write(it->second, msg, len);
	bufferevent_flush(it->second, EV_WRITE, BEV_NORMAL);
}

void	NetLoop::send(int linkid, int uri, Packet& packet) {
	Pack pk(SVID_DEAMON, uri);
	packet.marshall(pk);
	pk.pack();

	send(linkid, pk.getBuf(), pk.getLen());
}

void	NetLoop::remove(int linkid) {
	std::map<int, bufferevent*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		return;
	}

	bufferevent_free(it->second);
	m_mapLinks.erase(it);
}

int		NetLoop::getSystemTime() {
	timeval tv;
	evutil_gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

void	NetLoop::tcp_listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	sockaddr_in* addr = (sockaddr_in*)sa;	
	NetLoop *loop = (NetLoop*)user_data;
	bufferevent *bev = NULL;

	LOG(TAG_DEAMON, "receive connect, fd/ip/port=%d, %d:%d.", fd, addr->sin_addr.s_addr, addr->sin_port);
	bev = bufferevent_socket_new(loop->m_event_base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LOG(TAG_DEAMON, "NetLoop::listener_cb, fail on bufferevent_socket_new.");
		event_base_loopbreak(loop->m_event_base);
		return;
	}
	bufferevent_setcb(bev, tcp_read_cb, NULL, tcp_event_cb, loop);	
	bufferevent_enable(bev, EV_READ);

	int linkid = bufferevent_getfd(bev); 
	loop->m_mapLinks[linkid] = bev;
}

void	NetLoop::tcp_read_cb(struct bufferevent *bev, void *ctx)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_DEAMON, "NetLoop::read_cb, seq=%d", seq++);	
#endif

	NetLoop* loop = (NetLoop*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_DEAMON, "NetLoop::read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMDMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_DEAMON, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::tcp_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_DEAMON, "NetLoop::write_cb");	
}

void	NetLoop::tcp_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);
	
	if (events & BEV_EVENT_EOF || events & BEV_EVENT_ERROR ) {
		LOG(TAG_DEAMON, "disconnect from linkid %d", linkid);
		loop->remove(linkid);

	} else if (events & BEV_EVENT_CONNECTED) {
		LOG(TAG_DEAMON, "connected for linkid %d", linkid);		
	}
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMDMgr->onTimer();
}