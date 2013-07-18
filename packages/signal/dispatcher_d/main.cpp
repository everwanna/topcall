#include "dispmgr.h"
#include "inifile.h"
#include "dispconfig.h"

int main(int argc, char* argv[])
{
	char* inifile = "dispatcher_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_DISPATCHER, "dispatcher_d, fail to open %s.", inifile);
		return -1;
	}

	DispConfig config;
	config.name = ini.getString("dispatcher", "name");
	config.ip = ini.getString("dispatcher", "ip");
	config.port = ini.getInt("dispatcher", "port");
	config.isp = ini.getInt("dispatcher", "isp");
	config.area = ini.getInt("dispatcher", "area");
	config.disp_ip = ini.getString("dispatcher", "disp_ip");
	config.disp_port = ini.getInt("dispatcher", "disp_port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_DISPATCHER, "dispatcher_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_DISPATCHER, "dispatcher_d, running at configuration: ");
	LOG(TAG_DISPATCHER, "	name    = %s ", config.name.c_str());
	LOG(TAG_DISPATCHER, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_DISPATCHER, "	port    = %d ", config.port);
	LOG(TAG_DISPATCHER, "	area    = %d ", config.area);
	LOG(TAG_DISPATCHER, "	master_ip   = %s ", config.disp_ip.c_str());
	LOG(TAG_DISPATCHER, "	master_port = %d ", config.disp_port);

	DispMgr* mgr = new DispMgr(config);
	mgr->run();

	return 0;
}
