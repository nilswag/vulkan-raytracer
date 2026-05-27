#include "state.h"
#include "util/log.h"
#include "vulkan/app.h"

State state = { 0 };

static void loop(void)
{
	//INFO("Hello World!");
}

int main(void)
{
	App* app = &state.vulkan_app;
	vulkan_app_init(app, 800, 800, "Vulkan");
	vulkan_app_run(app, loop);
	vulkan_app_deinit(app);

	return 0;
}