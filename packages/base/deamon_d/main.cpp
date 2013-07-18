// mcenter_d.cpp : Defines the entry point for the console application.
//
#include "mdmgr.h"


int main(int argc, char* argv[])
{
	MDMgr* mgr = new MDMgr(PORT_DEAMON);

	mgr->run();

	return 0;
}
