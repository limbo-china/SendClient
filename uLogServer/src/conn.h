#ifndef _CONN_H_
#define _CONN_H_

#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include "log.h"

typedef enum
{
	CONN_REUSE			=	1 << 0,
	CONN_NONBLOCK		=	1 << 1,
	CONN_ALIVE			=	1 << 2,
	CONN_DEFER			=	1 << 3,
	CONN_OTHER			=	1 << 10
} conn_flag_t;

/**
 * @brief 创建服务器监听套接字
 * @param conn	封装连接的结构体
 * @param ip	服务器IP地址
 * @param port	服务器监听的端口	
 * @param mask	连接状态掩码
 * @return 
 *		大于0	成功
 *		-1		失败
 */
int conn_open(const char *ip, uint16_t port, int mask);

/**
 * @brief 创建客户端连接套接字
 * @param conn	封装连接的结构体
 * @param ip	服务器端IP地址
 * @param port	服务器端监听的端口
 * @param mask	连接状态掩码
 * @return 
 *		大于0	成功
 *		-1		失败
 */
int conn_peer(const char *ip, uint16_t port, int mask);

/**
 * @brief	检查描述符是否有错误
 *
 */
int conn_check(int fd);

/**
 *
 */
ssize_t conn_recv_tv(int fd, void *buf, size_t nbytes, struct timeval *tv);

/**
 *
 *
 */
int conn_read(int fd, void *buf, size_t len, int timeout);

/**
 *
 */
ssize_t conn_send_tv(int fd, void *buf, size_t nbytes, struct timeval *tv);

/**
 *
 *
 */
int conn_write(int fd, void *buf, size_t len, int timeout);

ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n);
#endif /*_CONN_H_*/
