#ifndef _ERROR_H_
#define _ERROR_H_

#include <unistd.h>
#include <string.h>

typedef enum
{
	G_RET_SUCCESS       =   0, //读写OK
	G_RET_EPARAM        =  -1, //参数有问题
	G_RET_EBODYLEN      =  -2, //变长数据长度有问题
	G_RET_READ          =  -3, //读消息体失败，具体错误看errno
	G_RET_READHEAD      =  -4, //读消息头失败, 具体错误看errno
	G_RET_WRITE         =  -5, //写的问题
	G_RET_WRITEHEAD     =  -6, //写消息头失败, 可能是对方将连接关闭了
	G_RET_PEERCLOSE     =  -7, //对端关闭连接
	G_RET_ETIMEDOUT     =  -8, //读写超时
	G_RET_EMAGICNUM     =  -9, //magic不匹配
	G_RET_UNKNOWN       =  -10 //未知错误
} error_flag;

/**
 * @brief	
 *
 */
const char *get_errstr(int ret);

/**
 *
 *
 */
int show_errstr(int ret);

#endif /*_ERROR_H_*/
