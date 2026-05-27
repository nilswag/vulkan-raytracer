#pragma once
#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow* window;
} App;

void vulkan_app_init(App* self);