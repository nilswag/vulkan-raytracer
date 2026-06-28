#include <string>
#include <GLFW/glfw3.h>
#include "vk_app.h"
#include "vk_instance.h"
#include "vk_device.h"
#include "../util/log.h"

App::App(const AppInfo& app_info)
{
    logger::Debug("App: initializing");

    width = app_info.width;
    height = app_info.height;

    if (!glfwInit())
        logger::Fatal("App: glfwInit failed");
    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, app_info.title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        logger::Fatal("App: glfwCreateWindow failed");

    instance.init(app_info);

    Device device = Device::get_device(instance, 0, std::vector<const char*> { VK_KHR_SWAPCHAIN_EXTENSION_NAME });

    logger::Debug("App: initialized");
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    logger::Debug("App: deinitialized");
}

void App::run()
{
    logger::Trace("App: entering main loop");

    double last = glfwGetTime();
    double timer = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        // rendering

        // updates
        double now = glfwGetTime();
        dt = now - last;
        last = now;

        timer += dt;
        if (timer >= 1.0)
        {
            logger::Info("DT = {:.3f} ms, FPS = {:.1f}", dt * 1000.0, 1.0 / dt);
            timer -= 1.0;
        }

        glfwPollEvents();
        ticks++;
    }
}