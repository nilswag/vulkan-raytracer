#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "app.h"
#include "../util/log.h"

void vulkan_app_init(App* s, int width, int height, const char* title)
{
	if (!glfwInit()) FATAL("Unable to initialize GLFW");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	s->window = glfwCreateWindow(width, height, title, NULL, NULL);
	s->width = width;
	s->height = height;

	if (!s->window) FATAL("Unable to initialize GLFW window");

	DEBUG("Initialized Vulkan app");
}

void vulkan_app_deinit(App* s)
{
	glfwDestroyWindow(s->window);
	glfwTerminate();
	*s = (App){ 0 };
	DEBUG("Deinitialized Vulkan app");
}

void vulkan_app_run(App* s, void (*loop)(void))
{
	while (!glfwWindowShouldClose(s->window))
	{
		glfwPollEvents();

		loop();

		glfwSwapBuffers(s->window);
	}
}