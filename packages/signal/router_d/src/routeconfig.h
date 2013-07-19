#ifndef _ROUTECONFIG_H_
#define _ROUTECONFIG_H_

#include <string>

struct RouteConfig {
	std::string name;
	std::string ip;
	int		port;
	int		isp;
	int		area;


	std::string	router1_ip;
	short		router1_port;
	std::string	router2_ip;
	short		router2_port;
	std::string	router3_ip;
	short		router3_port;
	std::string	router4_ip;
	short		router4_port;
	
};

#endif
