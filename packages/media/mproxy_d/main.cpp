// mproxy_udp_d.cpp : Defines the entry point for the console application.
//

#include "mpmgr.h"
#include "mpconfig.h"
#include "inifile.h"

void usage();
int main(int argc, char* argv[])
{
	const char* inifile = "mproxy_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG("mproxy_udp_d, fail to open %s.", inifile);
		return -1;
	}

	MPConfig config;
	config.name = ini.getString("mproxy", "name");
	config.ip = ini.getString("mproxy", "ip");
	config.port = ini.getInt("mproxy", "port");
	config.isp = ini.getInt("mproxy", "isp");
	config.area = ini.getInt("mproxy", "area");
	config.load_max = ini.getInt("mproxy", "load_max");
	config.mg_ip = ini.getString("mgroup", "ip");
	config.mg_port = ini.getInt("mgroup", "port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0 ||
		config.load_max == 0 ||
		config.mg_ip.length() == 0 ||
		config.mg_port == 0 ) {
		LOG(TAG_MPROXY, "mproxy_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_MPROXY, "mproxy_d, running at configuration: ");
	LOG(TAG_MPROXY, "	name    = %s ", config.name.c_str());
	LOG(TAG_MPROXY, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_MPROXY, "	port    = %d ", config.port);
	LOG(TAG_MPROXY, "	area    = %d ", config.area);
	LOG(TAG_MPROXY, "	load_max= %d ", config.load_max);
	LOG(TAG_MPROXY, "	group_ip   = %s ", config.mg_ip.c_str());
	LOG(TAG_MPROXY, "	group_port = %d ", config.mg_port);

	MPMgr* mp = new MPMgr(config);
	mp->run();

	return 0;
}

void usage() {
}
