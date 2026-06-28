#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "vk_instance.h"

struct AppInfo
{
    int width_;
    int height_;
    std::string title_;
};

class App
{
public:
    App(const AppInfo& app_info);
    ~App();

    void Run();

private:
    Instance instance_;

    GLFWwindow* window_;
    int width_, height_;
    size_t ticks_;
    double dt_;
};