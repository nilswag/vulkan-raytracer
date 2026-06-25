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

    create_physical_device();
    create_logical_device();
    
    logger::debug("Device: initialized");
}

VkDeviceQueueCreateInfo Device::create_queue_family_ci()
{
    logger::trace("Device: creating queue family");

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    uint32_t queue_family = -1;
    for (size_t i = 0; i < queue_families.size(); i++)
    {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_family = i;
            break;
        }
    }
    if (queue_family < 0)
        logger::error("Device: queue family flag unsupported 'VK_QUEUE_GRAPHICS_BIT'");

    // const float queue_priorities = 1.0f;
    VkDeviceQueueCreateInfo queue_ci = {};
    queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_ci.queueFamilyIndex = queue_family;
    queue_ci.queueCount = 1;
    queue_ci.pQueuePriorities = nullptr;

    return queue_ci;
}

void Device::create_physical_device()
{
    logger::trace("Device: creating physical device");

    VkPhysicalDeviceProperties2 physical_device_properties = {};
    physical_device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physical_device_properties.pNext = nullptr;
    vkGetPhysicalDeviceProperties2(physical_device, &physical_device_properties);
    logger::info("Device: using '{}'", physical_device_properties.properties.deviceName);
}

void Device::create_logical_device()
{
    logger::trace("Device: creating logical device");
    
    VkPhysicalDeviceFeatures enabled_vk10_features = {};
    enabled_vk10_features.samplerAnisotropy = VK_TRUE;

    VkPhysicalDeviceVulkan12Features enabled_vk12_features = {};
    enabled_vk12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    enabled_vk12_features.descriptorIndexing = true;
    enabled_vk12_features.shaderSampledImageArrayNonUniformIndexing = true;
    enabled_vk12_features.descriptorBindingVariableDescriptorCount = true;
    enabled_vk12_features.runtimeDescriptorArray = true;
    enabled_vk12_features.bufferDeviceAddress = true;

    VkPhysicalDeviceVulkan13Features enabled_vk13_features = {};
    enabled_vk13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    enabled_vk13_features.pNext = &enabled_vk12_features;
    enabled_vk13_features.synchronization2 = true;
    enabled_vk13_features.dynamicRendering = true;

    VkDeviceQueueCreateInfo queue_ci = create_queue_family_ci();

    VkDeviceCreateInfo device_ci = {};
}