#ifndef CONFIG_H
#define CONFIG_H

#ifdef WIN32
#define MEAN_AND_LEAN
#include <WinSock2.h>
#pragma warning(disable:4996)
#else
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <core/packet.h>
#include <core/res.h>
#include <core/log.h>
#include <core/svid.h>
#include <core/port.h>

#include <protocol/pmcenter.h>

extern "C" {
	#include <event2/event.h>
	#include <event2/listener.h>
	#include <event2/buffer.h>
	#include <event2/bufferevent.h>
}

#define TAG_MCPROXY				"mcproxy - "
#define NET_BUFFER_SIZE			1024*128
#define TIMER_INTERVAL			1
#define MC_RECONN_INTERVAL		5
#define CACHE_EXPIRE_INTERVAL	20
#define CACHE_SIZE				300
#define CACHE_MINIMAL			50

#endif
