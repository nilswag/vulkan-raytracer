#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class App
{

public:
     App(int width, int height, const std::string& title);
     ~App();

     void run();

private:
     GLFWwindow* window;
     VkInstance instance;

     size_t ticks = 0;
     double dt = 0.0;
};