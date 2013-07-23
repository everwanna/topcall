#ifndef _UIDSYNC_H_
#define _UIDSYNC_H_

#include "config.h"
#include <vector>

class LoginMgr;
class UidSync {
public:
	UidSync(LoginMgr* mgr);

public:
	void	onAdd(int uid);
	void	onRemvoe(int uid);
	void	sync();

private:
	LoginMgr*	m_pLoginMgr;
	std::vector<int>	m_arrAdds;
	std::vector<int>	m_arrRemoves;
};

#endif

