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

     int width, height;
     std::string title;

     size_t ticks = 0;
     double dt = 0.0;

     void init_instance();
     void init_device();
};