#ifndef _PUSHCONFIG_H_
#define _PUSHCONFIG_H_

#include <string>

struct PullConfig {
	std::string name;
	std::string ip;
	int		port;

	std::string mongo_ip;
	short		mongo_port;
};

#endif
