#include <vector>
#include <vulkan/vulkan.h>
#include "vk_device.h"
#include "vk_instance.h"
#include "../util/log.h"

Device Device::GetDevice(Instance& instance, uint32_t index, const std::vector<const char*>& requested_device_extensions)
{
    uint32_t device_count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(instance.Get(), &device_count, nullptr), "Device", "vkEnumeratePhysicalDevices");
    std::vector<VkPhysicalDevice> physical_devices(device_count);
    VK_CHECK(vkEnumeratePhysicalDevices(instance.Get(), &device_count, physical_devices.data()), "Device", "vkEnumeratePhysicalDevices");

    if (index >= physical_devices.size())
        LOG_FATAL("Device: index out of bounds");

    return Device(instance, physical_devices[index], requested_device_extensions);
}

Device::Device(Instance& instance, VkPhysicalDevice& physical_device, const std::vector<const char*>& requested_device_extensions)
    : physical_device_(physical_device), requested_device_extensions_(requested_device_extensions)
{
    CreatePhysicalDevice();
    CreateLogicalDevice();    
}

Device::~Device()
{
    vkDestroyDevice(logical_device_, nullptr);
}

VkDeviceQueueCreateInfo Device::GetQueueFamilyCI(uint32_t& queue_family)
{
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

    queue_family = UINT32_MAX;
    for (size_t i = 0; i < queue_families.size(); i++)
    {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_family = i;
            break;
        }
    }
    if (queue_family == UINT32_MAX)
        LOG_ERROR("Device: queue family flag unsupported 'VK_QUEUE_GRAPHICS_BIT'");

    VkDeviceQueueCreateInfo queue_ci = {};
    queue_ci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_ci.queueFamilyIndex = queue_family;
    queue_ci.queueCount = 1;

    return queue_ci;
}

void Device::CreatePhysicalDevice()
{
    VkPhysicalDeviceProperties2 physical_device_properties = {};
    physical_device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physical_device_properties.pNext = nullptr;
    vkGetPhysicalDeviceProperties2(physical_device_, &physical_device_properties);
    LOG_DEBUG("Device: created physical device using '{}'", physical_device_properties.properties.deviceName);
}

void Device::CreateLogicalDevice()
{    
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

    uint32_t queue_family;
    VkDeviceQueueCreateInfo queue_ci = GetQueueFamilyCI(queue_family);
    const float queue_priorities = 1.0f;
    queue_ci.pQueuePriorities = &queue_priorities;

    VkDeviceCreateInfo device_ci = {};
    device_ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_ci.pNext = &enabled_vk13_features;
    device_ci.queueCreateInfoCount = 1;
    device_ci.pQueueCreateInfos = &queue_ci;
    device_ci.enabledExtensionCount = static_cast<uint32_t>(requested_device_extensions_.size());
    device_ci.ppEnabledExtensionNames = requested_device_extensions_.data();
    device_ci.pEnabledFeatures = &enabled_vk10_features;

    VK_CHECK(vkCreateDevice(physical_device_, &device_ci, nullptr, &logical_device_), "Device", "vkCreateDevice");
    vkGetDeviceQueue(logical_device_, queue_family, 0, &queue_);
    LOG_DEBUG("Device: created logical device");
}
