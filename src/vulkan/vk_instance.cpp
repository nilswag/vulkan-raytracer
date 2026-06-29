#include <string>
#include <vector>
#include <cstring>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "vk_instance.h"
#include "vk_app.h"
#include "../util/log.h"

static VkBool32 DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
)
{
    std::string message_type_str;
    if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
        message_type_str = "GENERAL";
    else if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        message_type_str = "VALIDATION";
    else if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        message_type_str = "PERFORMANCE";


    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        LOG_TRACE("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);

    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        LOG_TRACE("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);

    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        LOG_TRACE("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);

    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        LOG_TRACE("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);

    return VK_FALSE;
}

void Instance::Init(const AppInfo& app_info)
{    
    VkApplicationInfo vk_app_info = {};
    vk_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vk_app_info.pApplicationName = app_info.title_.c_str();
    vk_app_info.pEngineName = "No engine";
    vk_app_info.apiVersion = VK_API_VERSION_1_3;

    uint32_t glfw_extensions_count = 0;
    const char** glfw_extensions_ptr = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);
    requested_extensions_.insert(requested_extensions_.end(), glfw_extensions_ptr, glfw_extensions_ptr + glfw_extensions_count);

#ifdef _DEBUG
    requested_extensions_.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    requested_layers_.push_back("VK_LAYER_KHRONOS_validation");
#endif

    ValidateExtensions();
    ValidateLayers();

    VkInstanceCreateInfo instance_ci = {};
    instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_ci.pApplicationInfo = &vk_app_info;
    instance_ci.enabledExtensionCount = static_cast<uint32_t>(requested_extensions_.size());
    instance_ci.ppEnabledExtensionNames = requested_extensions_.data();
#ifdef _DEBUG
    instance_ci.enabledLayerCount = static_cast<uint32_t>(requested_layers_.size());
    instance_ci.ppEnabledLayerNames = requested_layers_.data();
#endif

    VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &instance_), "Instance", "vkCreateInstance");

#ifdef _DEBUG
    CreateDebugMessenger();
#endif

    LOG_DEBUG("Instance: created instance");
}

Instance::~Instance()
{
#ifdef _DEBUG
    DestroyDebugMessenger();
#endif

    vkDestroyInstance(instance_, nullptr);
}

void Instance::CreateDebugMessenger()
{
    LOG_TRACE("Instance: creating debug messenger");

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_ci = {};
    debug_messenger_ci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                   | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                   | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_ci.pfnUserCallback = DebugMessengerCallback;

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT"));

    VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance_, &debug_messenger_ci, nullptr, &debug_messenger_), "Instance", "vkCreateDebugUtilsMessengerEXT");
}

void Instance::DestroyDebugMessenger()
{
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT"));    
    vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
}

void Instance::ValidateLayers()
{
    LOG_TRACE("Instance: checking validation layer support");

    uint32_t layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "Instance", "vkEnumerateInstanceLayerProperties");
    std::vector<VkLayerProperties> available_layers(layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()), "Instance", "vkEnumerateInstanceLayerProperties");

    for (const char*& layer_name : requested_layers_)
    {
        if (std::find_if(available_layers.begin(), available_layers.end(), [layer_name](const VkLayerProperties& layer) {
            return std::strcmp(layer_name, layer.layerName) == 0;
        }) == available_layers.end())
        {
            LOG_WARN("Instance: validation layer '{}' not found", layer_name);
            continue;
        }

        LOG_DEBUG("Instance: validation layer '{}' supported", layer_name);
    }
}

void Instance::ValidateExtensions()
{
    LOG_TRACE("Instance: checking extension support");

    uint32_t extension_count = 0;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr), "Instance", "vkEnumerateInstanceExtensionProperties");
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data()), "Instance", "vkEnumerateInstanceExtensionProperties");

    for (const char*& extension_name : requested_extensions_)
    {
        if (std::find_if(available_extensions.begin(), available_extensions.end(), [extension_name](const VkExtensionProperties& extension) {
            return std::strcmp(extension_name, extension.extensionName) == 0;
        }) == available_extensions.end())
        {
            LOG_WARN("Instance: extension '{}' not found", extension_name);
            continue;
        }

        LOG_DEBUG("Instance: extension '{}' supported", extension_name);
    }
}