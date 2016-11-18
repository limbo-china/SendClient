#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

uint32_t get_msec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

int ms2tv(int msec, struct timeval *tv)
{
	if(tv == NULL){
		return -1;
	}

	tv->tv_sec = msec / 1000;
	tv->tv_usec = (msec % 1000) * 1000;

	return 0;
}

int time2str(char *tmbuf, size_t size, const char *fmt)
{
	time_t local;
	struct tm now;
	struct timeval tv;

	local = time(NULL);
	gettimeofday(&tv, NULL);
	localtime_r(&local, &now);
	strftime(tmbuf, size, fmt, &now);
	size = strlen(tmbuf);
	sprintf(tmbuf+size,"%03d",(int)tv.tv_usec/1000);
	return 0;
}

int time2str2(time_t local, char *tmbuf, size_t size, const char *fmt)
{
	struct tm now;

	localtime_r(&local, &now);
	strftime(tmbuf, size, fmt, &now);

	return 0;
}
