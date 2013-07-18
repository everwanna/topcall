// mcenter_d.cpp : Defines the entry point for the console application.
//
#include "mcpmgr.h"

void usage();

int main(int argc, char* argv[])
{
	if( argc < 4 ) {
		usage();
		return -1;
	}

	char* mc_ip = argv[1];
	int mc_port = atoi(argv[2]);
	int port = atoi(argv[3]);
	if( port < PORT_MCPROXY1 || port > PORT_MCPROXY9 ) {
		LOG(TAG_MCPROXY, "mcproxy port should be [2041,2049]");
		return -1;
	}


	MCPMgr* mgr = new MCPMgr(mc_ip, mc_port, port);
	mgr->run();

	return 0;
}

void usage() {
	LOG(TAG_MCPROXY, "mcproxy_d, the proxy for mcenter.");
	LOG(TAG_MCPROXY, "use it as: ./mproxy_d {mc_ip} {mc_port} {port}");
	LOG(TAG_MCPROXY, "mc_ip : mcenter ip");
	LOG(TAG_MCPROXY, "mc_port : mcenter port (2021)");
	LOG(TAG_MCPROXY, "port range should be [2041.2049]");
}
