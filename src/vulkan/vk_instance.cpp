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
    switch (message_severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        logger::trace("Validation layer: {}", callback_data->pMessage);
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        logger::debug("Validation layer: {}", callback_data->pMessage);
        break;
        
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        logger::warn("Validation layer: {}", callback_data->pMessage);
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        logger::error("Validation layer: {}", callback_data->pMessage);
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
    std::vector<const char*> required_extensions(extensions_ptr, extensions_ptr + extension_count);

#ifdef _DEBUG
    required_extensions.push_back("VK_EXT_debug_utils");
#endif

    logger::debug("Instance: required extensions ({}):", required_extensions.size());
    for (int i = 0; i < required_extensions.size(); i++)
        logger::debug("  [{}] = '{}'", i, required_extensions[i]);

    VkInstanceCreateInfo instance_ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &vk_app_info,
        .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
        .ppEnabledExtensionNames = required_extensions.data(),
    };

#ifdef _DEBUG
    add_validation_layer("VK_LAYER_KHRONOS_validation");
    check_validation_layers();

    instance_ci.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    instance_ci.ppEnabledLayerNames = validation_layers.data();
#endif

    if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
        logger::fatal("Instance: vkCreateInstance failed");

    logger::debug("Instance: initialized");
}

Instance::~Instance()
{
    vkDestroyInstance(instance, nullptr);
    logger::debug("Instance: deinitialized");
}

void Instance::add_validation_layer(const char* layer_name)
{
    logger::trace("Instance: adding validation layer '{}'", layer_name);
    validation_layers.push_back(layer_name);
}

void Instance::check_validation_layers()
{
    logger::trace("Instance: enabling validation layers");

    if (validation_layers.empty())
        return;

    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : validation_layers)
    {
        logger::trace("Instance: enabling validation layer '{}'", layer_name);
        if (available_layers.end() == std::find_if(available_layers.begin(), available_layers.end(),
            [layer_name](const VkLayerProperties& layer) {
                return std::strcmp(layer_name, layer.layerName) == 0;
            }))
        {
            logger::warn("Instance: validation layer '{}' not found", layer_name);
            continue;
        }
        
        logger::debug("Instance: validation layer '{}' enabled", layer_name);
    }
}