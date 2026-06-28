#include <cstdlib>
#include "vulkan/vk_app.h"
#include "util/log.h"

int main()
{
	try
	{
		App app({ .width_ = 1000, .height_ = 1000, .title_ = "Vulkan Raytracer" });
		app.Run();
	}
	catch (const std::exception& e)
	{
		if (e.what() != nullptr) LOG_DEBUG("{}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}