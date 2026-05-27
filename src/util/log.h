#pragma once
#include <string.h>
#include <stdlib.h>

typedef enum
{
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_N
} LogLevel;

void _log(LogLevel level, const char* file, int line, const char* msg, ...);

#define __FILE_BASE__ \
			(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 :   \
			 strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : \
			 __FILE__)

#ifdef _DEBUG

#define FATAL(msg, ...) do { _log(LOG_LEVEL_FATAL,   __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)
#define ERROR(msg, ...) do { _log(LOG_LEVEL_ERROR,   __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); } while (0)
#define WARN(msg, ...)  do { _log(LOG_LEVEL_WARNING, __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); } while (0)
#define INFO(msg, ...)  do { _log(LOG_LEVEL_INFO,    __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); } while (0)
#define DEBUG(msg, ...) do { _log(LOG_LEVEL_DEBUG,   __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); } while (0)
#define TRACE(msg, ...) do { _log(LOG_LEVEL_TRACE,   __FILE_BASE__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#else

#define FATAL(msg, ...)
#define ERROR(msg, ...)
#define WARN(msg, ...)
#define INFO(msg, ...)
#define DEBUG(msg, ...)
#define TRACE(msg, ...)

#endif