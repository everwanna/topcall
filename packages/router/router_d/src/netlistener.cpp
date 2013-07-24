#include "netlistener.h"
#include "netloop.h"
#include "routemgr.h"
#include "link.h"
#include "linkmgr.h"
#include "msghandler.h"

NetListener::NetListener(NetLoop* loop) 
	: m_pLooper(loop)
	, m_listener(NULL)
{
}

NetListener::~NetListener() {
	if( m_listener ) {
		evconnlistener_free( m_listener );
	}
}

void	NetListener::listen(short port) {
	LOG(TAG_ROUTER, "router listener on port=%d", port);
	//startup tcp listener:
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	m_listener = evconnlistener_new_bind( m_pLooper->getEventBase(), listener_cb, (void *)this,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sin,
		sizeof(sin));

}

void	NetListener::listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	NetListener *nl = (NetListener*)user_data;
	sockaddr_in* addr = (sockaddr_in*)sa;		
	bufferevent *bev = NULL;

	bev = bufferevent_socket_new(nl->m_pLooper->getEventBase(), fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LOG(TAG_ROUTER, "NetLoop::listener_cb, fail on bufferevent_socket_new.");
		event_base_loopbreak(nl->m_pLooper->getEventBase());
		return;
	}
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, nl);
	bufferevent_enable(bev, EV_READ|EV_WRITE);

	//add the link to the LinkMgr:
	int linkid = bufferevent_getfd(bev); 
	Link* link = new Link();
	link->linkid = linkid; 
	link->bev = bev;
	link->stamp = nl->m_pLooper->getSystemTime();
	link->closed = false;
	nl->m_pLooper->getMgr()->getLinkMgr()->addLink(link);

	LOG(TAG_ROUTER, "connect from linkid=%d, ip/port=%s, %d", linkid, inet_ntoa(addr->sin_addr), addr->sin_port);
}

void	NetListener::read_cb(struct bufferevent *bev, void *ctx)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_MCENTER, "NetLoop::read_cb, seq=%d", seq++);	
#endif

	NetListener* listener = (NetListener*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_ROUTER, "NetLoop::read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, listener->m_pLooper->getBuffer(), msg_len);

			listener->m_pLooper->getMgr()->getHandler()->handle(bufferevent_getfd(bev), listener->m_pLooper->getBuffer(), msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_ROUTER, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetListener::write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MCENTER, "NetLoop::write_cb");	
}

void	NetListener::event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetListener* listener = (NetListener*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_EOF || events & BEV_EVENT_ERROR) {
		LOG(TAG_ROUTER, "disconnected, linkid=%d", linkid);
		listener->m_pLooper->getMgr()->getLinkMgr()->remove(linkid);

	} else if ( events & BEV_EVENT_CONNECTED ) {
		LOG(TAG_ROUTER, "connected, linkid=%d", linkid);		
	}
}