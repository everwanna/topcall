#ifndef _MSG_H_
#define _MSG_H_

#include "config.h"

class Msg {
public:
	Msg();
	~Msg();

public:
	void	assign(char* buf, int len);

public:
	const char*	data() { return m_pData; }
	int		len() { return m_nLen; }

private:
	char*	m_pData;
	int		m_nCapacity;
	int		m_nLen;
};

#endif
