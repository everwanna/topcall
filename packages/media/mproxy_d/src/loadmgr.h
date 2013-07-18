#ifndef _LOADMGR_H_
#define _LOADMGR_H_

class MPMgr;
class LoadMgr {
public:
	LoadMgr(MPMgr* mgr, int max);
	~LoadMgr();

public:
	int		getLoad();
	int		getMaxLoad();
	bool	hasLoad();

private:
	MPMgr*	m_pMPMgr;
	int		m_nLoadMax;
};

#endif
