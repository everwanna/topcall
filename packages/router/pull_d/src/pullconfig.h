#ifndef _PUSHCONFIG_H_
#define _PUSHCONFIG_H_

#include <string>

struct PullConfig {
	std::string router_ip;
	short		router_port;
	
	std::string mongo_dbname;
	std::string mongo_ip;
	short		mongo_port;
};

#endif
