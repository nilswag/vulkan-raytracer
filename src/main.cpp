#include <cstdlib>
#include "vulkan/vk_app.h"

int main()
{
	try
	{
		VulkanApp app({ .width = 1000, .height = 1000, .title = "Vulkan Raytracer" });
		app.run();
	}
	catch (const std::exception& e)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}