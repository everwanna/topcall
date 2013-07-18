#include "mcmgr.h"
#include "grpmgr.h"
#include "mcconfig.h"
#include "inifile.h"

void	usage();
void	fill_groups(MCMgr* mgr);
void	select_group(MCMgr* mgr);

int main(int argc, char* argv[])
{
	char* inifile = "mcenter_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG("mproxy_udp_d, fail to open %s.", inifile);
		return -1;
	}

	MCConfig config;
	config.name = ini.getString("mcenter", "name");
	config.ip = ini.getString("mcenter", "ip");
	config.port = ini.getInt("mcenter", "port");
	config.isp = ini.getInt("mcenter", "isp");
	config.area = ini.getInt("mcenter", "area");
	config.deamon_ip = ini.getString("deamon", "ip");
	config.deamon_port = ini.getInt("deamon", "port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_MCENTER, "mcenter_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_MCENTER, "mcenter_d, running at configuration: ");
	LOG(TAG_MCENTER, "	name    = %s ", config.name.c_str());
	LOG(TAG_MCENTER, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_MCENTER, "	port    = %d ", config.port);
	LOG(TAG_MCENTER, "	area    = %d ", config.area);
	LOG(TAG_MCENTER, "	dm_ip   = %s ", config.deamon_ip.c_str());
	LOG(TAG_MCENTER, "	dm_port = %d ", config.deamon_port);

	MCMgr* mgr = new MCMgr(config);
	mgr->run();

	return 0;
}

void	usage() {
	LOG(TAG_MCENTER, "mcenter_d, the media center manager");
	LOG(TAG_MCENTER, "use it as: ./mcenter_d {port} or ");
	LOG(TAG_MCENTER, "			 ./mcenter_d {port} {whiltelist}");
	LOG(TAG_MCENTER, "");
}