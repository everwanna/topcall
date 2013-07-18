#ifndef _LTITEM_H_
#define _LTITEM_H_

#include <string>

struct LTItem {
	std::string	key;
	std::string	ip;
	short	port;
	int		load;
	int		load_max;
	std::string	ext;

	//runtime:
	int		score;
	int		reserve;
};


#endif
