#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "vk_instance.h"

struct AppInfo
{
    int width;
    int height;
    std::string title;
};

class App
{
public:
    App(const AppInfo& app_info);
    ~App();

    void run();

private:
    Instance instance = { 0 };

    GLFWwindow* window = nullptr;
    int width, height;
    size_t ticks = 0;
    double dt = 0.0;
};