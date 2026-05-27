#include <stdarg.h>
#include <stdio.h>
#include "log.h"

void _log(LogLevel level, const char* file, int line, const char* msg, ...)
{
	if (level < 0 || level >= LOG_LEVEL_N) return;

	static const char* prefixes[LOG_LEVEL_N] = {
		"FATAL",
		"ERROR",
		"WARN",
		"INFO",
		"DEBUG",
		"TRACE"
	};

	FILE* stream = level < LOG_LEVEL_WARNING ? stderr : stdout;

	fprintf(stream, "[%s][%s:%d] ", prefixes[level], file, line);

	va_list args;
	va_start(args, msg);
	vfprintf(stream, msg, args);
	va_end(args);
}
