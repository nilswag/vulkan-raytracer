#include <vector>
#include <vulkan/vulkan.h>
#include "vk_device.h"
#include "vk_instance.h"
#include "../util/log.h"

Device Device::get_device(Instance& instance, uint32_t index)
{
    logger::trace("Device: creating device with index {}", index);

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance.get(), &device_count, nullptr);
    std::vector<VkPhysicalDevice> physical_devices(device_count);
    vkEnumeratePhysicalDevices(instance.get(), &device_count, physical_devices.data());

    if (index >= physical_devices.size())
        logger::fatal("Device: index out of bounds");

    return Device(instance, physical_devices[index]);
}

Device::Device(Instance& instance, VkPhysicalDevice& physical_device)
    : physical_device(physical_device)
{
    logger::debug("Device: initializing");

    VkPhysicalDeviceProperties2 physical_device_properties;
    physical_device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    vkGetPhysicalDeviceProperties2(physical_device, &physical_device_properties);
    logger::info("Device: using '{}'", physical_device_properties.properties.deviceName);

    logger::debug("Device: initialized");
}