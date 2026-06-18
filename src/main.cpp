#include <cstdlib>
#include "util/log.h"

int main()
{
	try
	{
		logger::trace("Hello World!");
		logger::debug("Hello World!");
		logger::info("Hello World!");
		logger::warn("Hello World!");
		logger::error("Hello World!");
		logger::fatal("Hello World!");
	}
	catch (const std::exception& e)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}