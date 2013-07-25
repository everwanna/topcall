#ifndef _STATMGR_H_
#define _STATMGR_H_

#include "config.h"

class StatMgr {
public:
	StatMgr();

public:
	void	onUserLogin();
	void	onUserLogout();

public:
	int		getUserCount() { return m_nUserCount; }

private:
	int		m_nUserCount;

};

#endif
