#pragma once
#include <string>
#include <GLFW/glfw3.h>

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
    GLFWwindow* window = nullptr;
    int width, height;
};