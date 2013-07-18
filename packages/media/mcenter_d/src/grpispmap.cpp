#include "grpispmap.h"
#include "grplist.h"

GrpIspMap::GrpIspMap() {
}

GrpIspMap::~GrpIspMap() {
	//do nothing here, we don't own the GrpInfo.
}

void	GrpIspMap::add(int isp, int area, GrpInfo* info) {
	char	strisp[64] = {'\0'};
	char	strisparea[64] = {'\0'};

	sprintf(strisp, "%d", isp);
	sprintf(strisparea, "%d:%d", isp, area);
	
	//add to isp->grplist map.
	if( m_mapIspGroups.find(strisp) != m_mapIspGroups.end() ) {
		m_mapIspGroups[strisp]->addGroup(info);
	} else {
		GrpList* list = new GrpList();
		list->addGroup(info);
		m_mapIspGroups[strisp] = list;
	}	

	//add to isp:area ->grplist map:
	if( m_mapIspGroups.find(strisparea) != m_mapIspGroups.end() ) {
		m_mapIspGroups[strisparea]->addGroup(info);
	} else {
		GrpList* list = new GrpList();
		list->addGroup(info);
		m_mapIspGroups[strisparea] = list;
	}	
}

void	GrpIspMap::remove(int isp, int area, const std::string& group) {
	char	strisp[64] = {'\0'};
	char	strisparea[64] = {'\0'};

	sprintf(strisp, "%d", isp);
	sprintf(strisparea, "%d:%d", isp, area);
	if( m_mapIspGroups.find(strisp) != m_mapIspGroups.end() ) {
		m_mapIspGroups[strisp]->remvoeGroup(group);
	}

	if( m_mapIspGroups.find(strisparea) != m_mapIspGroups.end() ) {
		m_mapIspGroups[strisparea]->remvoeGroup(group);
	}
}

GrpInfo*	GrpIspMap::findGroup(int isp, int area, int num) {
	GrpInfo* info = NULL;
	char	strisp[64] = {'\0'};
	char	strisparea[64] = {'\0'};

	sprintf(strisp, "%d", isp);
	sprintf(strisparea, "%d:%d", isp, area);

	//find group match isp/area.
	if( m_mapIspGroups.find(strisparea) != m_mapIspGroups.end() ) {
		info = m_mapIspGroups[strisparea]->getFreeGroup(num);
	}
	if( info != NULL ) {
		return info;
	}

	if( m_mapIspGroups.find(strisp) != m_mapIspGroups.end() ) {
		info = m_mapIspGroups[strisp]->getFreeGroup(num);
	}
	if( info != NULL ) {
		return info;
	}

	return NULL;
}
