#pragma once
#include <string>
#include <vulkan/vulkan.h>
#include "vulkan_instance.h"

VulkanInstance::VulkanInstance(const std::string& application_name)
{
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = application_name.c_str(),
        .pEngineName = "No engine",
        .apiVersion = VK_API_VERSION_1_4,
    };

    VkInstanceCreateInfo instance_ci = {
        
    };
}

VulkanInstance::~VulkanInstance()
{

}