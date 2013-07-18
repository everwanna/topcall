#ifndef _UINFO_H_
#define _UINFO_H_

#include "config.h"

struct UInfo {
	int			uid;
	sockaddr_in	addr;
	std::string nick;
};

#endif