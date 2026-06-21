#pragma once
#include <string>
#include <vulkan/vulkan.h>

struct VulkanInstance
{
    VulkanInstance(const std::string& application_name);
    ~VulkanInstance();

    VkInstance instance = VK_NULL_HANDLE;
};