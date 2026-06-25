#include <cstdlib>
#include "vulkan/vk_app.h"
#include "util/log.h"

int main()
{
	try
	{
		App app({ .width = 1000, .height = 1000, .title = "Vulkan Raytracer" });
		app.run();
	}
	catch (const std::exception& e)
	{
		if (e.what() != nullptr) logger::debug("{}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}