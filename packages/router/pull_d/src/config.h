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

	#define MONGO_USE__INT64	1
	#define MONGO_STATIC_BUILD	1
	#include "mongo.h"
}


#define TAG_PULL			"pull - "
#define NET_BUFFER_SIZE		1024*1024*8
#define TIMER_INTERVAL		1

#endif
