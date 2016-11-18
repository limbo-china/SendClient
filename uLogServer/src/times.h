#ifndef _TIMES_H_
#define _TIMES_H_

#include <stdlib.h>
/**
 * @brief	get msec of current time
 * @return
 */
uint32_t get_msec();

/**
 * @brief	convert msec time to timeval
 * @param	msec
 * @param	tv
 * @return
 *			0	success
 *		   -1	fail
 */
int ms2tv(int msec, struct timeval *tv);

/**
 * @brief convert local time to string
 * @param	tmbuf	time buffer
 * @param	size	time buffer size
 * @param	fmt		output time by format
 * @return
 *			0	success
 */
int time2str(char *tmbuf, size_t size, const char *fmt);

/**
 * @brief convert local time to string
 * @param	local	local time
 * @param	tmbuf	time buffer
 * @param	size	time buffer size
 * @param	fmt		output time by format
 * @return
 *			0	success
 */
int time2str2(time_t local, char *tmbuf, size_t size, const char *fmt);

#endif /*_TIMES_H_*/
