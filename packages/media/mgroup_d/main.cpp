// mproxy_group_d.cpp : Defines the entry point for the console application.
//

#include "mgmgr.h"
#include "proxymgr.h"
#include "inifile.h"
#include "mgconfig.h"

int main(int argc, char* argv[])
{
	char* inifile = "mgroup_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_MGROUP, "mgroup_d, fail to open %s.", inifile);
		return -1;
	}

	MGConfig config;
	config.name = ini.getString("mgroup", "name");
	config.ip = ini.getString("mgroup", "ip");
	config.port = ini.getInt("mgroup", "port");
	config.isp = ini.getInt("mgroup", "isp");
	config.area = ini.getInt("mgroup", "area");	

	config.mc_ip = ini.getString("mcenter", "ip");
	config.mc_port = ini.getInt("mcenter", "port");

	config.lt_ip = ini.getString("loadtable", "ip");
	config.lt_port = ini.getInt("loadtable", "port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0 ||		
		config.mc_ip.length() == 0 ||
		config.mc_port == 0 ) {
		LOG(TAG_MGROUP, "mgroup_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_MGROUP, "mgroup_d, running at configuration: ");
	LOG(TAG_MGROUP, "	name    = %s ", config.name.c_str());
	LOG(TAG_MGROUP, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_MGROUP, "	port    = %d ", config.port);
	LOG(TAG_MGROUP, "	area    = %d ", config.area);	
	LOG(TAG_MGROUP, "	mc_ip   = %s ", config.mc_ip.c_str());
	LOG(TAG_MGROUP, "	mc_port = %d ", config.mc_port);
	LOG(TAG_MGROUP, "	lt_ip   = %s ", config.lt_ip.c_str());
	LOG(TAG_MGROUP, "	lt_port = %d ", config.lt_port);

	MGMgr* mgr = new MGMgr(config);
	mgr->run();

	return 0;
}