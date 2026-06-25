#include <string>
#include <vector>
#include <cstring>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "vk_instance.h"
#include "vk_app.h"
#include "../util/log.h"

static VkBool32 debug_messenger_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
)
{
    std::string message_type_str;
    if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
        message_type_str = "GENERAL";

    if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        message_type_str = "VALIDATION";

    if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        message_type_str = "PERFORMANCE";

    switch (message_severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        logger::trace("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        logger::debug("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);
        break;
        
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        logger::warn("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        logger::error("Validation layer [{}]: {}", message_type_str, callback_data->pMessage);
        break;
    }

    return VK_FALSE;
}

void Instance::init(const AppInfo& app_info)
{
    logger::debug("Instance: initializing");
    
    VkApplicationInfo vk_app_info = {};
    vk_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vk_app_info.pApplicationName = app_info.title.c_str();
    vk_app_info.pEngineName = "No engine";
    vk_app_info.apiVersion = VK_API_VERSION_1_3;

    uint32_t glfw_extensions_count = 0;
    const char** glfw_extensions_ptr = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);
    requested_extensions.insert(requested_extensions.end(), glfw_extensions_ptr, glfw_extensions_ptr + glfw_extensions_count);

#ifdef _DEBUG
    requested_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    requested_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    validate_extensions();
    validate_layers();

    VkInstanceCreateInfo instance_ci = {};
    instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    instance_ci.pApplicationInfo = &vk_app_info,
    instance_ci.enabledExtensionCount = static_cast<uint32_t>(requested_extensions.size()),
    instance_ci.ppEnabledExtensionNames = requested_extensions.data(),
#ifdef _DEBUG
    instance_ci.enabledLayerCount = static_cast<uint32_t>(requested_layers.size());
    instance_ci.ppEnabledLayerNames = requested_layers.data();
#endif

    if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
        logger::fatal("Instance: vkCreateInstance failed");

#ifdef _DEBUG
    create_debug_messenger();
#endif

    logger::debug("Instance: initialized");
}

Instance::~Instance()
{
#ifdef _DEBUG
    destroy_debug_messenger();
#endif

    vkDestroyInstance(instance, nullptr);
    logger::debug("Instance: deinitialized");
}

void Instance::create_debug_messenger()
{
    logger::trace("Instance: creating debug messenger");

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_ci = {};
    debug_messenger_ci.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                   | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                   | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_ci.pfnUserCallback = debug_messenger_callback;

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_ci, nullptr, &debug_messenger) != VK_SUCCESS)
        logger::error("Instance: vkCreateDebugUtilsMessengerEXT failed");
}

void Instance::destroy_debug_messenger()
{
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));    
    vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
}

void Instance::validate_layers()
{
    logger::trace("Instance: checking validation layer support");

    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char*& layer_name : requested_layers)
    {
        if (std::find_if(available_layers.begin(), available_layers.end(), [layer_name](const VkLayerProperties& layer) {
            return std::strcmp(layer_name, layer.layerName) == 0;
        }) == available_layers.end())
        {
            logger::warn("Instance: validation layer '{}' not found", layer_name);
            continue;
        }

        logger::debug("Instance: validation layer '{}' supported", layer_name);
    }
}

void Instance::validate_extensions()
{
    logger::trace("Instance: checking extension support");

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

    for (const char*& extension_name : requested_extensions)
    {
        if (std::find_if(available_extensions.begin(), available_extensions.end(), [extension_name](const VkExtensionProperties& extension) {
            return std::strcmp(extension_name, extension.extensionName) == 0;
        }) == available_extensions.end())
        {
            logger::warn("Instance: extension '{}' not found", extension_name);
            continue;
        }

        logger::debug("Instance: extension '{}' supported", extension_name);
    }
}