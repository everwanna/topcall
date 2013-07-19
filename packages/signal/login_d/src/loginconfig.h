#ifndef _LTCONFIG_H_
#define _LTCONFIG_H_

#include <string>

struct LoginConfig {
	std::string name;
	std::string ip;
	int		port;
	int		isp;
	int		area;


	std::string	disp_ip;
	short		disp_port;

	std::string udb_ip;
	short  udb_port;
	
};

#endif
