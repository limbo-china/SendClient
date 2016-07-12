#ifndef _LOG_H_
#define _LOG_H_

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include "stdafx.h"
typedef struct log_s log_t;
typedef void (*log_cleanup_cb)(log_t *log);

typedef enum
{
	LOG_EMERG = 0,
	LOG_ERROR = 1,
	LOG_ALERT = 2,
	LOG_WARNING = 3,
	LOG_INFO = 4,
	LOG_DEBUG
} level_t;

struct log_s
{
	char *path;
	char *suffix;

	char hour[20];
	FILE *fp;
	level_t level;

	log_cleanup_cb cleanup;
};

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_DEBUG
#endif

#define log_emerg(log, ...)												\
	do{																	\
		log_inner(log, LOG_EMERG, __FILE__, __LINE__, __VA_ARGS__);		\
	} while(0)

#define log_error(log, ...)												\
	do{																	\
		log_inner(log, LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);		\
	} while(0)

#define log_alert(log, ...)												\
	do{																	\
		log_inner(log, LOG_ALERT, __FILE__, __LINE__, __VA_ARGS__);		\
	} while(0)

#define log_warn(log, ...)												\
	do{																	\
		log_inner(log, LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__);	\
	} while(0)

#define log_info(log, ...)												\
	do{																	\
		log_inner(log, LOG_INFO, __FILE__, __LINE__, __VA_ARGS__);		\
	} while(0)

#define log_debug(log, ...)												\
	do{																	\
		log_inner(log, LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__);		\
	} while(0)

log_t *log_create(const char *path, const char *suffix, level_t level);

extern int log_inner(log_t *log, level_t level, const char *file,
		const long line, const char *fmt, ...);

void log_destroy(log_t *log);

#endif /*_LOG_H_*/
