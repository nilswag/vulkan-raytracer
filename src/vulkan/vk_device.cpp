#include <vector>
#include <vulkan/vulkan.h>
#include "vk_device.h"
#include "vk_instance.h"
#include "../util/log.h"

Device Device::get_device(Instance& instance, uint32_t index, const std::vector<const char*>& requested_device_extensions)
{
    logger::trace("Device: creating device with index {}", index);

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance.get(), &device_count, nullptr);
    std::vector<VkPhysicalDevice> physical_devices(device_count);
    vkEnumeratePhysicalDevices(instance.get(), &device_count, physical_devices.data());

    if (index >= physical_devices.size())
        logger::fatal("Device: index out of bounds");

    return Device(instance, physical_devices[index], requested_device_extensions);
}

Device::Device(Instance& instance, VkPhysicalDevice& physical_device, const std::vector<const char*>& requested_device_extensions)
    : physical_device(physical_device)
{
    logger::debug("Device: initializing");

    VkPhysicalDeviceProperties2 physical_device_properties = {};
    physical_device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physical_device_properties.pNext = nullptr;

    vkGetPhysicalDeviceProperties2(physical_device, &physical_device_properties);
    logger::info("Device: using '{}'", physical_device_properties.properties.deviceName);

    logger::debug("Device: initialized");
}

void Device::create_queue()
{
    logger::trace("Device: creating queue");

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    uint32_t queue_family = 0;
    for (size_t i = 0; i < queue_families.size(); i++)
    {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_family = i;
            break;
        }
    }
    if (queue_family == 0)
        logger::fatal("Device: queue family unsupported 'VK_QUEUE_GRAPHICS_BIT'");
    
    const float queue_priorities = 1.0f;
    VkDeviceQueueCreateInfo queue_ci = {};
    queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_ci.queueFamilyIndex = queue_family;
    queue_ci.queueCount = 1;
    queue_ci.pQueuePriorities = &queue_priorities;
}