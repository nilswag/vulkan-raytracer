#pragma once
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "vulkan_instance.h"
#include "vulkan_app.h"
#include "../util/log.h"

void VulkanInstance::init(const VulkanAppInfo& app_info)
{
    logger::debug("Initializing VulkanInstance...");

    VkApplicationInfo vk_app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_info.title.c_str(),
        .pEngineName = "No engine",
        .apiVersion = VK_API_VERSION_1_4,
    };

    uint32_t extension_count = 0;
    const char** extensions_ptr = glfwGetRequiredInstanceExtensions(&extension_count);
    std::vector<const char*> extensions(extensions_ptr, extensions_ptr + extension_count);
    logger::info("Enabled extensions ({}):", extension_count);
    for (int i = 0; i < extensions.size(); i++)
        logger::info("  [{}] = {}", i, extensions[i]);

    VkInstanceCreateInfo instance_ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &vk_app_info
    };

    if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
        logger::fatal("Unable to initialize vulkan instance");

    logger::debug("Initialized VulkanInstance");
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(instance, nullptr);
    logger::debug("Deinitialized VulkanInstance");
}