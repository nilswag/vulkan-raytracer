#include <cstdlib>
#include "util/log.h"

int main()
{
	logger::out(logger::LogLevel::DEBUG, "aaa {}", 123);

	return EXIT_SUCCESS;
}