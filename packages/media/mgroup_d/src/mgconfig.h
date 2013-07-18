#ifndef _MGCONFIG_H_
#define _MGCONFIG_H_

struct MGConfig {
	std::string name;
	std::string ip;
	short		port;
	int			isp;
	int			area;
	std::string mc_ip;
	short		mc_port;
	std::string lt_ip;
	short		lt_port;
};

#endif
