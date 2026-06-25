#include <string>
#include <GLFW/glfw3.h>
#include "vk_app.h"
#include "vk_instance.h"
#include "vk_device.h"
#include "../util/log.h"

App::App(const AppInfo& app_info)
{
    logger::debug("App: initializing");

    width = app_info.width;
    height = app_info.height;

    if (!glfwInit())
        logger::fatal("App: glfwInit failed");
    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, app_info.title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        logger::fatal("App: glfwCreateWindow failed");

    instance.init(app_info);

    Device::get_device(instance, 0);

    logger::debug("App: initialized");
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    logger::debug("App: deinitialized");
}

void App::run()
{
    logger::trace("App: entering main loop");

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
            logger::info("DT = {:.3f} ms, FPS = {:.1f}", dt * 1000.0, 1.0 / dt);
            timer -= 1.0;
        }

        glfwPollEvents();
        ticks++;
    }
}