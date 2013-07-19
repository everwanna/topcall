#ifndef _DISPCONFIG_H_
#define _DISPCONFIG_H_

#include <string>

struct DispConfig {
	std::string name;
	std::string ip;
	int		port;
	int		isp;
	int		area;


	std::string	router_ip;
	short		router_port;
	
};

#endif
