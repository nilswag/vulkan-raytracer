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

	static const char* colors[LOG_LEVEL_N] = {
		"\033[0;31m",
		"\033[0;31m",
		"\033[0;33m",
		"\033[1;37m",
		"\033[0;36m",
		"\033[0;35m"
	};

	FILE* stream = level < LOG_LEVEL_WARNING ? stderr : stdout;

	fprintf(stream, "%s[%s][%s:%d] ", colors[level], prefixes[level], file, line);

	va_list args;
	va_start(args, msg);
	vfprintf(stream, msg, args);
	va_end(args);

	fprintf(stream, "\033[0;0m\n");
}
