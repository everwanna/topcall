#include "netloop.h"
#include "pushmgr.h"
#include "msghandler.h"

NetLoop::NetLoop(PushMgr* mgr, short port) : m_pPushMgr(mgr), m_port(port)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif
	m_event_base = event_base_new();

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);

	m_listener = evconnlistener_new_bind(m_event_base, listener_cb, (void *)this,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	m_pBuffer = new char[NET_BUFFER_SIZE];
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

void	NetLoop::listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	LOG(TAG_PUSH, "NetLoop::listener_cb.");
	NetLoop *loop = (NetLoop*)user_data;
	bufferevent *bev;

	bev = bufferevent_socket_new(loop->m_event_base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(loop->m_event_base);
		return;
	}
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, loop);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);

	int linkid = bufferevent_getfd(bev); 
	loop->m_mapLinks[linkid] = bev;
}

void	NetLoop::read_cb(struct bufferevent *bev, void *ctx)
{
//	static int seq = 0;
//	LOG(TAG_PUSH, "NetLoop::read_cb, seq=%d", seq++);	
	NetLoop* loop = (NetLoop*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len == 0 )
			break;
		if( len <= 4 ) {
			LOG(TAG_PUSH, "NetLoop::read_cb, len<=4");	
			break;
		}

		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {
			evbuffer_remove(src, loop->m_pBuffer, msg_len);
			loop->m_pPushMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);						
		} else {
			//not enough data again, return.
			LOG(TAG_PUSH, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_PUSH, "NetLoop::write_cb");	
}

void	NetLoop::event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_EOF || events &BEV_EVENT_ERROR ) {
		LOG(TAG_PUSH, "NetLoop::event_cb, BEV_EVENT_EOF for linkid %d", linkid);
		loop->m_pPushMgr->deleteConsumer(linkid);
	}
}