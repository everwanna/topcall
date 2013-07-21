#ifndef _LINK_H_
#define _LINK_H_

#include "config.h"

struct Link {
	int		linkid;
	int		uid;
	int		seq;
	int		stamp;
	bufferevent* bev;
	bool	closed;
};

#endif
