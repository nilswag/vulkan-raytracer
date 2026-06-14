#include <stdint.h>
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

	// vulkan stuff

	// === instance setup ===
	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = title,
		.apiVersion = VK_API_VERSION_1_3	
	};

	// get amount and names of enabled extensions
	uint32_t extension_count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extension_count);

	// vulkan instance create info
	VkInstanceCreateInfo instance_ci = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledExtensionCount = extension_count,
		.ppEnabledExtensionNames = extensions
	};

	if (vkCreateInstance(&instance_ci, NULL, &s->instance) != VK_SUCCESS)
		FATAL("Unable to create vulkan instance");
	TRACE("Created vulkan instance");
	// ===============================

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