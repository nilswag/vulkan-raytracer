#pragma once
#include <string>
#include <vulkan/vulkan.h>

struct VulkanAppInfo;

struct VulkanInstance
{
    VulkanInstance(VulkanAppInfo& app_info);
    ~VulkanInstance();

    VkInstance instance = VK_NULL_HANDLE;
};