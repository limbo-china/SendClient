#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "util.h"

int strrpl(char *s, char sval, char dval)
{
	char *q = s;

	for(char *p=s; *p!='\0'; p++)
	{
		if(*p == sval){
			q = p;
			*q = dval;
		}
	}

	return 0;
}

uint32_t gen_ran(uint32_t low, uint32_t high)
{
	if(low > high)
		return -1;

	return rand()%(high-low+1) + low;
}

uint32_t cton(uint32_t cidr)
{
	uint32_t mask;
	uint32_t bits = sizeof(uint32_t) * 8;

	if(cidr >= bits)	return 0;

	mask = ~0;
	bits -= cidr;
	mask <<= bits;

	return htonl(mask);
}

uint64_t htonll(uint64_t host)
{
	uint64_t net = 0;
	uint32_t lh, rh;
	uint32_t ln, rn;

	lh = (host & 0xFFFFFFFF00000000) >> 32;
	rh = host & 0x00000000FFFFFFFF;

	ln = htonl(lh);
	rn = htonl(rh);

	net = rn;
	net = (net << 32) + ln;

	return net;
}

uint64_t ntohll(uint64_t net)
{
	uint64_t host = 0;
	uint32_t lh, rh;
	uint32_t ln, rn;

	ln = (net & 0xFFFFFFFF00000000) >> 32;
	rn = net & 0x00000000FFFFFFFF;

	lh = ntohl(ln);
	rh = ntohl(rn);

	host = rh;
	host = (host << 32) + lh;

	return host;
}

int get_cpus()
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}

int get_sockflag(int sockfd)
{
	int flag = -1;

	while((flag=fcntl(sockfd, F_GETFL, 0)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}

	return flag;
}

void print_hex(const char *data, size_t len)
{
	unsigned char c;

	for(size_t i=0; i<len; i++)
	{
		c = data[i];
		printf("%.2x ", c);
		if(i % 16 == 15)
			putchar('\n');
	}
	putchar('\n');
}

int set_rlimit(int n)
{
	struct rlimit limit;

	limit.rlim_cur = n;
	limit.rlim_max = n;

	if(setrlimit(RLIMIT_NOFILE, &limit) == -1){
		perror("setrlimit");
		return -1; 
	}   

	return 0;
}

int set_reuse(int sockfd)
{
	int reuse = 1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_blocking(int sockfd)
{
	int val = 0;
	int flags, mask;

	while((flags=fcntl(sockfd, F_GETFL, 0)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}
	mask = flags;
	if(!(mask & O_NONBLOCK)){
		return 0;
	}
	flags &= ~O_NONBLOCK;

	while((val=fcntl(sockfd, F_SETFL, flags)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}

	return val;
}

int set_nonblocking(int sockfd)
{
	int opts;

	opts=fcntl(sockfd, F_GETFL);
	if(opts < 0)
		return -1;

	opts = opts | O_NONBLOCK;

	if(fcntl(sockfd, F_SETFL, opts) < 0){
		return -1;
	}

#if 0
	while((opts=fcntl(sockfd, F_GETFL)) == -1)
	{
		if(errno == EINTR)
			continue;
		return -1;
	}

	opts = opts | O_NONBLOCK;

	while(fcntl(sockfd, F_SETFL, opts) == -1){   
		if(errno == EINTR)
			continue;
		return -1; 
	}   
#endif

	return 0;
}

int set_timeout(int sockfd, int sec, int flag)
{
	struct timeval tv;

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	if(flag == 0){
		if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(struct timeval)) != 0){
			perror("setsockopt");
			return -1;
		}
	}else if(flag == 1){
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(struct timeval)) != 0){
			perror("setsockopt");
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}

int set_keepalive(int sockfd)
{
	int alive = 1;

	if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&alive, sizeof(alive)) != 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_defer(int sockfd)
{
	int delay = 3;

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &delay, sizeof(delay)) != 0){
		perror("setsockopt");
		return -1;
	}

	return 0;
}

int set_affinity(int cpuid, pid_t pid)
{
	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(cpuid, &mask);

	if(sched_setaffinity(pid, sizeof(cpu_set_t), &mask) == -1){
		perror("sched_setaffinity");
		return -1;
	}

	return 0;
}
