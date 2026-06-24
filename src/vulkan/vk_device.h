#pragma once
#include <vulkan/vulkan.h>
#include "vk_instance.h"

class Device
{
public:
    static Device get_device(Instance& instance, uint32_t index);

private:
    Device(Instance& instance, VkPhysicalDevice& physical_device);

    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties2 physical_device_properties;
    VkDevice logical_device;
};