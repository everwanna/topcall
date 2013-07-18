#ifndef _LTCONFIG_H_
#define _LTCONFIG_H_

#include <string>

struct LTConfig {
	std::string name;
	std::string ip;
	int		port;
	int		isp;
	int		area;


	std::string	master_ip;
	short		master_port;
	
};

#endif
