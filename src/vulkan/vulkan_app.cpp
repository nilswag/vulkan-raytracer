#include <string>
#include <GLFW/glfw3.h>
#include "vulkan_app.h"
#include "../util/log.h"

VulkanApp::VulkanApp(const VulkanAppInfo& app_info)
{
    width = app_info.width;
    height = app_info.height;

    if (!glfwInit())
        logger::fatal("Unable to initialize GLFW");
    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, app_info.title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        logger::fatal("Unable to initialize GLFW window");

    logger::debug("VulkanApp initialized");
}

VulkanApp::~VulkanApp()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    logger::debug("VulkanApp deinitialized");
}

void VulkanApp::run()
{
    logger::trace("Started VulkanApp");
    
    while (!glfwWindowShouldClose(window))
    {
        // rendering

        // updates
        glfwPollEvents();
    }
}