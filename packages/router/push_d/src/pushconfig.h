#ifndef _PUSHCONFIG_H_
#define _PUSHCONFIG_H_

#include <string>

struct PushConfig {
	std::string name;
	std::string ip;
	int		port;

	int			mongo_enable;
	std::string mongo_dbname;
	std::string mongo_ip;
	short		mongo_port;
};

#endif
