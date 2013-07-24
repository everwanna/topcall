// media_push_d.cpp : Defines the entry point for the console application.
//
#include "netloop.h"
#include "pushmgr.h"

void usage();

int main(int argc, char* argv[])
{
	int port = 2001;

	usage();
	if( argc == 2 ) {
		port = atoi(argv[1]);
	}

	printf("");
	printf("Starting idc_push_d with port: %d.....\r\n", port);
	PushMgr* mgr = new PushMgr(port);
	mgr->createTopic("100");
	mgr->createTopic("101");

	printf("Running  idc_push_d with port: %d.....\r\n", port);
	mgr->run();
	return 0;
}

void usage() {
	printf("idc_push_d is a service to simulate message queue in producer&consumer mode.\r\n");
	printf("There can be multiple Producer and multiple Consumer.\r\n");
	printf("Producer send message to a topic.\r\n");
	printf("And Consumer subscribe to a topic.\r\n");
	printf("Message is send to a Topic.\r\n");
	printf("");
	printf("The default port is 2001, you can change it with following: \r\n");	
	printf("\t./idc_push_d 2001\r\n");	
}
