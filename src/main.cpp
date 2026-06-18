#include <cstdlib>
#include "util/log.h"

int main()
{
	logger::trace("Hello World!");
	logger::debug("Hello World!");
	logger::info("Hello World!");
	logger::warn("Hello World!");
	logger::error("Hello World!");
	logger::fatal("Hello World!");

	return EXIT_SUCCESS;
}