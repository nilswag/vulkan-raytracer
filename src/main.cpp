#include <cstdlib>
#include "vulkan/vulkan_app.h"

int main()
{
	try
	{
		VulkanApp app(100, 1000, "vulkan raytracer");
		app.run();
	}
	catch (const std::exception& e)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}