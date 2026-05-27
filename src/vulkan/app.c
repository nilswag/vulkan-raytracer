#include <GLFW/glfw3.h>
#include "app.h"
#include "../util/log.h"

void vulkan_app_init(App* self)
{
	if (glfwInit()) FATAL("Unable to initialize GLFW");
}