#ifndef _MSG_H_
#define _MSG_H_

#include "config.h"

struct Msg {
	int		uid;
	int		seq;
	int		retry;
	std::string	payload;
};

#endif
