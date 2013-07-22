#ifndef _SEQ_H_
#define _SEQ_H_

#include "config.h"

class Seq {
public:
	Seq();

public:
	int		current();
	int		next();

private:
	int		m_nSeq;
};


#endif
