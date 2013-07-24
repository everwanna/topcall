#ifndef CONFIG_H
#define CONFIG_H

#include <core/res.h>
#include <core/log.h>
#include <core/packet.h>


#include <protocol/ppush.h>

extern "C" {
	#include <event2/event.h>
	#include <event2/listener.h>
	#include <event2/buffer.h>
	#include <event2/bufferevent.h>
}

#define TAG_PUSH			"push - "
#define NET_BUFFER_SIZE		1024*1024*8

#endif
