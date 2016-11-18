#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "times.h"

#ifndef atomic_add
#define atomic_add(x, y) (__sync_add_and_fetch(&(x), (y)))
#endif
#ifndef atomic_sub
#define atomic_sub(x, y) (__sync_sub_and_fetch(&(x), (y)))
#endif

int strrpl(char *s, char sval, char dval);

uint32_t gen_ran(uint32_t low, uint32_t high);

uint32_t cton(uint32_t cidr);
uint64_t htonll(uint64_t net);
uint64_t ntohll(uint64_t host);

int get_cpus();
int get_sockflag(int sockfd);
void print_hex(const char *data, size_t len);

int set_rlimit(int n);
int set_reuse(int sockfd);
int set_blocking(int sockfd);
int set_nonblocking(int sockfd);
int set_timeout(int sockfd, int sec, int flag);
int set_defer(int sockfd);
int set_keepalive(int sockfd);
int set_affinity(int cpuid, pid_t pid);

#endif /*_UTIL_H_*/
