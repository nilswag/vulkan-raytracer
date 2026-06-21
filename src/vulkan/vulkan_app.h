#pragma once
#include <string>

struct GLFWwindow;

class VulkanApp
{
public:
    VulkanApp(int width, int height, const std::string& title);
    ~VulkanApp();

    void run();

private:
    GLFWwindow* window = nullptr;
    int width;
    int height;
    std::string title;
};