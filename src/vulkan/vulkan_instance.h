#pragma once
#include <string>
#include <vulkan/vulkan.h>

struct VulkanAppInfo;

struct VulkanInstance
{
    void init(const VulkanAppInfo& app_info);
    ~VulkanInstance();

    VkInstance instance = VK_NULL_HANDLE;
};