#ifndef _CYCLE_H_
#define _CYCLE_H_

#include <string.h>

#include "log.h"


typedef struct
{
	log_t *syslog;
	char target_path[256];
	char keys_path[256];
	char local_ip[20];
	int local_port;
} cycle_t;

extern cycle_t g_cycle;

#endif /*_CYCLE_H_*/
