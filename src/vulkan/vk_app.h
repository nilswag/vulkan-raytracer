#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "vk_instance.h"

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
    size_t ticks = 0;
    double dt = 0.0;
};