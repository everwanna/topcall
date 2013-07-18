#include "grplist.h"
#include "grpinfo.h"

GrpList::GrpList() {
}

GrpList::~GrpList() {
	//[TBD] clean up.
}

void	GrpList::addGroup(GrpInfo* group) {
	m_arrGroups.push_back(group);
}

void	GrpList::remvoeGroup(const std::string& group) {
	for( std::list<GrpInfo*>::iterator it=m_arrGroups.begin(); it!=m_arrGroups.end(); it++ ) {
		if( group == (*it)->getName() ) {
			m_arrGroups.erase(it);
			break;
		}
	}
}

//find the first or find the best?
//score=load*100/load_max.
GrpInfo*	GrpList::getFreeGroup(int num) {	
	GrpInfo* info = NULL;
	GrpInfo* infotmp = NULL;
	int score = 0;

	for( std::list<GrpInfo*>::iterator it=m_arrGroups.begin(); it!=m_arrGroups.end(); it++ ) {
		infotmp = *it;

		//it's a good low-load server, return it immediatelly;
		//the critiral is: load<30% or left_load > 100K
		if( infotmp->getScore()>= 3 || infotmp->getLoadMax()-infotmp->getLoad()>10000*10 ) {
			return infotmp;
		}

		if( infotmp->getScore() > score ) {
			info = infotmp;
			score = info->getScore();
		}
	}

	return info;
}
