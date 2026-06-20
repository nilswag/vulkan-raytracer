#pragma once
#include <string>
#include <vector>
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
     VkDebugUtilsMessengerEXT debug_messenger;

     const std::vector<const char*> required_validation_layers = {
          "VK_LAYER_KHRONOS_validation"
     };

     int width, height;
     std::string title;

     size_t ticks = 0;
     double dt = 0.0;

     void init_validation_layers();
     void init_debug_messenger();
     void init_instance();
     void init_device();
};