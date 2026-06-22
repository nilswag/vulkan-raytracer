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
    switch (message_type)
    {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        message_type_str = "GENERAL";
        break;

        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        message_type_str = "VALIDATION";
        break;

        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        message_type_str = "PERFORMANCE";
        break;
    }

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

    VkApplicationInfo vk_app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_info.title.c_str(),
        .pEngineName = "No engine",
        .apiVersion = VK_API_VERSION_1_4,
    };

    uint32_t extension_count = 0;
    const char** extensions_ptr = glfwGetRequiredInstanceExtensions(&extension_count);


#ifdef _DEBUG
    extensions.push_back("VK_EXT_debug_utils");

    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
    
    for (const char* extension_name : extensions)
    {
        if (available_extensions.end() == std::find_if(available_extensions.begin(), available_extensions.end(),
            [extension_name](const VkExtensionProperties& extension) {
                return std::strcmp(extension_name, extension.extensionName) == 0;
            }))
        {
            logger::fatal("Instance: required extension '{}' not found", extension_name);
        }
    }
#endif

    logger::debug("Instance: required extensions ({}):", extensions.size());
    for (int i = 0; i < extensions.size(); i++)
        logger::debug("  [{}] = '{}'", i, extensions[i]);

    VkInstanceCreateInfo instance_ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &vk_app_info,
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

#ifdef _DEBUG
    add_validation_layer("VK_LAYER_KHRONOS_validation");
    check_validation_layers();

    instance_ci.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    instance_ci.ppEnabledLayerNames = validation_layers.data();
#endif

    if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
        logger::fatal("Instance: vkCreateInstance failed");

#ifdef _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_ci = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debug_messenger_callback,
    };

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_ci, nullptr, &debug_messenger) != VK_SUCCESS)
        logger::error("Instance: vkCreateDebugUtilsMessengerEXT failed");
#endif

    logger::debug("Instance: initialized");
}

Instance::~Instance()
{
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));    
    vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);

    vkDestroyInstance(instance, nullptr);
    logger::debug("Instance: deinitialized");
}

void Instance::create_debug_messenger()
{

}

void Instance::validate_layers()
{
    logger::trace("Instance: checking validation layer support");

    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char*& layer_name : validation_layers)
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

void Instance::validate_extension()
{
    
}

void Instance::add_validation_layer(const char* layer_name)
{
    logger::trace("Instance: adding validation layer '{}'", layer_name);
    validation_layers.push_back(layer_name);
}