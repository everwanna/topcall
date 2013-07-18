import os
import shutil 
import glob

def deldebug(path):
	try:
		if os.path.exists(path):					
			shutil.rmtree(path)
			
	except (IOError, os.error) as why:
		print str(why)
		pass

def rmfile(path):
	try:
		if os.path.exists(path):
			os.remove(path)
	except (IOError, os.error) as why:
		print str(why)
		pass

rmfile("packages/base/deamon_d/deamon_d.sdf")
deldebug("packages/base/deamon_d/debug")

rmfile("packages/base/loadtable_d/loadtable.sdf")
deldebug("packages/base/loadtable_d/debug")

rmfile("packages/media/mcenter_d/mcenter_d.sdf")
deldebug("packages/media/mcenter_d/debug")

rmfile("packages/media/mcproxy_d/mcproxy_d.sdf")
deldebug("packages/media/mcproxy_d/debug")

rmfile("packages/media/mgroup_d/mgroup_d.sdf")
deldebug("packages/media/mgroup_d/debug")

rmfile("packages/media/mgproxy_d/mgproxy_d.sdf")
deldebug("packages/media/mgproxy_d/debug")

rmfile("packages/media/mproxy_d/mproxy_d.sdf")
deldebug("packages/media/mproxy_d/debug")

rmfile("packages/signal/login_d/login_d.sdf")
deldebug("packages/signal/login_d/debug")