#include <stdlib.h>
#include <stdio.h>

#include "getconfigfromtext.h"
#include "cycle.h"

#define DEAULT_CONF_FILE "./conf/sys.cfg"

int sys_init()
{
	char buffer[50];
	memset(&g_cycle, 0, sizeof(g_cycle));
	GetConfigValue(DEAULT_CONF_FILE, "target_path", g_cycle.target_path);
	if(strlen(g_cycle.target_path) == 0)
	{
		printf("[ERROR]: get target_path error\n");
		return -1;
	}
	printf("%s\n", g_cycle.target_path);
	GetConfigValue(DEAULT_CONF_FILE, "keys_path", g_cycle.keys_path);
	if(strlen(g_cycle.keys_path) == 0)
	{
		printf("[ERROR]: get keys_path error\n");
		return -1;
	}

	printf("%s\n", g_cycle.keys_path);
	GetConfigValue(DEAULT_CONF_FILE, "local_ip", g_cycle.local_ip);
	if(strlen(g_cycle.local_ip) == 0)
	{
		printf("[ERROR]: get local_ip error\n");
		return -1;
	}
	printf("%s\n", g_cycle.local_ip);

	GetConfigValue(DEAULT_CONF_FILE, "local_port", buffer);
	g_cycle.local_port = (unsigned short)atoi(buffer);
	if(g_cycle.local_port == 0||g_cycle.local_port > 65535)
	{
		printf("[ERROR]: get local_port error\n");
		return -1;
	}
	printf("%d\n", g_cycle.local_port);

	return 0;
}

int main(int argc, char *argv[])
{
	if(sys_init() == -1)
	{
		printf("[ERROR]: Init error\n");
		return -1;
	}
	server_cnf_loop(g_cycle.local_ip, g_cycle.local_port);
	return 0;
}
