#ifndef _MPCONFIG_H_
#define _MPCONFIG_H_

#include <string>

struct MCConfig {
	std::string name;
	std::string ip;
	int port;
	int isp;
	int area;

	std::string deamon_ip;
	short	deamon_port;
};

#endif
