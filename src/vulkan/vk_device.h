#pragma once
#include <vulkan/vulkan.h>
#include "vk_instance.h"

class Device
{
public:
    static Device GetDevice(Instance& instance, uint32_t index, const std::vector<const char*>&);
    ~Device();

private:
    Device(Instance& instance, VkPhysicalDevice& physical_device, const std::vector<const char*>&);
    VkDeviceQueueCreateInfo GetQueueFamilyCI(uint32_t& queue_family);
    void CreatePhysicalDevice();
    void CreateLogicalDevice();

    std::vector<const char*> requested_device_extensions_;

    VkPhysicalDevice physical_device_;
    VkDevice logical_device_;
    VkQueue queue_;

};