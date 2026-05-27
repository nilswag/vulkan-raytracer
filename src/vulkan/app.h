#pragma once
#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow* window;
	int width, height;
} App;

void vulkan_app_init(App* app, int width, int height, const char* title);
void vulkan_app_deinit(App* app);
void vulkan_app_run(App* app, void (*loop)(void));