#ifndef _UINFO_H_
#define _UINFO_H_

#include <string>

struct UInfo {
	int		uid;	
	std::string	passport;
	std::string password;
	std::string cookie;
	std::string	router;
	std::string dispatcher;
	std::string proxy;

};

#endif
