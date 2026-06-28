#include <string>
#include <GLFW/glfw3.h>
#include "vk_app.h"
#include "vk_instance.h"
#include "vk_device.h"
#include "../util/log.h"

App::App(const AppInfo& app_info)
{
    width_ = app_info.width_;
    height_ = app_info.height_;

    if (!glfwInit())
        LOG_FATAL("App: glfwInit failed");
    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(width_, height_, app_info.title_.c_str(), nullptr, nullptr);
    if (window_ == nullptr)
        LOG_FATAL("App: glfwCreateWindow failed");

    instance_.Init(app_info);

    Device device = Device::GetDevice(instance_, 0, std::vector<const char*> { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
}

App::~App()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void App::Run()
{
    LOG_TRACE("App: entering main loop");

    double last = glfwGetTime();
    double timer = 0.0;

    while (!glfwWindowShouldClose(window_))
    {
        // rendering

        // updates
        double now = glfwGetTime();
        dt_ = now - last;
        last = now;

        timer += dt_;
        if (timer >= 1.0)
        {
            LOG_INFO("DT = {:.3f} ms, FPS = {:.1f}", dt_ * 1000.0, 1.0 / dt_);
            timer -= 1.0;
        }

        glfwPollEvents();
        ticks_++;
    }
}