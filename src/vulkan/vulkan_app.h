#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "vulkan_instance.h"

struct VulkanAppInfo
{
    int width;
    int height;
    std::string title;
};

class VulkanApp
{
public:
    VulkanApp(const VulkanAppInfo& app_info);
    ~VulkanApp();

    void run();

private:
    VulkanInstance instance = { 0 };

    GLFWwindow* window = nullptr;
    int width, height;

};