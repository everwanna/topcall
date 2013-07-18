#include "ltmgr.h"
#include "inifile.h"
#include "ltconfig.h"

int main(int argc, char* argv[])
{
	char* inifile = "loadtable.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_LTABLE, "loadtable, fail to open %s.", inifile);
		return -1;
	}

	LTConfig config;
	config.name = ini.getString("loadtable", "name");
	config.ip = ini.getString("loadtable", "ip");
	config.port = ini.getInt("loadtable", "port");
	config.isp = ini.getInt("loadtable", "isp");
	config.area = ini.getInt("loadtable", "area");
	config.master_ip = ini.getString("loadtable", "master_ip");
	config.master_port = ini.getInt("loadtable", "master_port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_LTABLE, "loadtable, the config file is invalid.");
		return -1;
	}

	LOG(TAG_LTABLE, "loadtable, running at configuration: ");
	LOG(TAG_LTABLE, "	name    = %s ", config.name.c_str());
	LOG(TAG_LTABLE, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_LTABLE, "	port    = %d ", config.port);
	LOG(TAG_LTABLE, "	area    = %d ", config.area);
	LOG(TAG_LTABLE, "	master_ip   = %s ", config.master_ip.c_str());
	LOG(TAG_LTABLE, "	master_port = %d ", config.master_port);

	LTMgr* mgr = new LTMgr(config);
	mgr->run();

	return 0;
}
