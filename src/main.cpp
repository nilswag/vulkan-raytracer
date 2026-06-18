#include <cstdlib>
#include "vulkan/app.h"

int main()
{
	try
	{
		App app(1000, 1000, "vulkan app");
		app.run();
	}
	catch (const std::exception& e)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}