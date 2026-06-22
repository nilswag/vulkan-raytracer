#include <string>
#include <GLFW/glfw3.h>
#include "vk_app.h"
#include "vk_instance.h"
#include "../util/log.h"

VulkanApp::VulkanApp(const VulkanAppInfo& app_info)
{
    logger::debug("VulkanApp: initializing");

    width = app_info.width;
    height = app_info.height;

    if (!glfwInit())
        logger::fatal("VulkanApp: glfwInit failed");
    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, app_info.title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        logger::fatal("VulkanApp: glfwCreateWindow failed");

    instance.init(app_info);

    logger::debug("VulkanApp: initialized");
}

VulkanApp::~VulkanApp()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    logger::debug("VulkanApp: deinitialized");
}

void VulkanApp::run()
{
    logger::trace("VulkanApp: entering main loop");

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