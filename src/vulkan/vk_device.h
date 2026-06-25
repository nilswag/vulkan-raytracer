#pragma once
#include <vulkan/vulkan.h>
#include "vk_instance.h"

class Device
{
public:
    static Device get_device(Instance& instance, uint32_t index, const std::vector<const char*>&);

private:
    Device(Instance& instance, VkPhysicalDevice& physical_device, const std::vector<const char*>&);
    void create_queue();

    VkPhysicalDevice physical_device;
    VkDevice logical_device;


};