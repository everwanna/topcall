#ifndef _MPCONFIG_H_
#define _MPCONFIG_H_

#include <string>

struct MPConfig {
	std::string name;
	std::string ip;
	int port;
	int isp;
	int area;
	int load_max;

	std::string mg_ip;
	int mg_port;
};

#endif
