#ifndef _ROUTECONFIG_H_
#define _ROUTECONFIG_H_

#include <string>

struct RouteConfig {
	std::string name;
	std::string ip;
	int		port;
	int		isp;
	int		area;
	
	std::string dm_ip;
	short		dm_port;
};

#endif
