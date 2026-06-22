#pragma once
#include <string>
#include <vulkan/vulkan.h>

struct AppInfo;

struct Instance
{
    void init(const AppInfo& app_info);
    ~Instance();

    VkInstance instance = VK_NULL_HANDLE;
};