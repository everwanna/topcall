#ifndef REPORT_H
#define REPORT_H

#include "config.h"

class MPMgr;
class Report {
public:
	Report(MPMgr* mgr);
	~Report();

public:
	void	report();

private:
	void	sendReport(PMGReportReq* reg);

private:
	MPMgr*		m_pMPMgr;
	int			m_nSocket;
};

#endif
