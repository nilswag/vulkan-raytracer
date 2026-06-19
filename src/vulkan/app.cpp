#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "app.h"
#include "../util/log.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
     VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
)
{
     std::string type_str;
     switch (type)
     {
          case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:     type_str = "GENERAL";     break;
          case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:  type_str = "VALIDATION";  break;
          case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: type_str = "PERFORMANCE"; break;
          default: type_str = "MISC"; break;
     }

     switch (severity)
     {
          case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
          logger::trace("vulkan diagnostic ({}): {}", type_str, callback_data->pMessage);
          break;

          case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
          logger::debug("vulkan info ({}): {}", type_str, callback_data->pMessage);
          break;

          case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
          logger::warn("vulkan warning ({}): {}", type_str, callback_data->pMessage);
          break;

          case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
          logger::error("vulkan error ({}): {}", type_str, callback_data->pMessage);

          default:
          logger::debug("vulkan misc ({})S: {}", type_str, callback_data);
          break;
     }

     return VK_FALSE;
}

void App::init_validation_layers()
{
     // list of wanted validation layers
     const std::vector<const char*> required_layers = {
          "VK_LAYER_KHRONOS_validation"
     };

     uint32_t layer_count = 0;
     vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
     std::vector<VkLayerProperties> available_layers(layer_count);
     vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

     for (const char* layer_name : required_layers)
     {
          auto it = std::find_if(available_layers.begin(), available_layers.end(), [layer_name](const auto& layer_properties){
               return strcmp(layer_name, layer_properties.layerName) == 0;
          });

          if (it == available_layers.end())
               logger::fatal("unsupported validation layer found ({})", layer_name);
     }
}

static PFN_vkCreateDebugUtilsMessengerEXT create_debug_utils_messenger_ext(
     VkInstance instance, 
     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
     const VkAllocationCallbacks* pAllocator, 
     VkDebugUtilsMessengerEXT* pDebugMessenger
)
{

}

void App::init_debug_messenger()
{
     VkDebugUtilsMessengerEXT debug_messenger;
     VkDebugUtilsMessengerCreateInfoEXT create_info = {
          .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
          .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT,
          .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT,
          .pfnUserCallback = vulkan_debug_callback,
          .pUserData = nullptr
     };

     PFN_vkCreateDebugUtilsMessengerEXT create_func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
     if (create_func == nullptr)
          logger::fatal("vulkan debug messenger extension not present");
     if (create_func(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
          logger::fatal("unable to set up vulkan debug messenger");

     logger::trace("initialized vulkan debug messenger");
}

void App::init_instance()
{
     VkApplicationInfo app_info = {
          .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
          .pApplicationName = title.c_str(),
          .apiVersion = VK_API_VERSION_1_3
     };

     uint32_t extension_count = 0;
     const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
     std::vector<const char*> required_extensions(glfw_extensions, glfw_extensions + extension_count);

     #ifdef _DEBUG
     required_extensions.push_back("VK_EXT_debug_utils");
     #endif

     logger::trace("found {} required extensions:", extension_count);
     for (int i = 0; i < extension_count; i++)
          logger::trace("  [{}] = {}", i, required_extensions[i]);

     VkInstanceCreateInfo instance_ci = {
          .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
          .pApplicationInfo = &app_info,
          .enabledExtensionCount = required_extensions.size(),
          .ppEnabledExtensionNames = required_extensions.data()
     };

     if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
          logger::fatal("unable to initialize vulkan instance");

     logger::trace("initialized vulkan instance");
}

void App::init_device()
{
     uint32_t device_count = 0;
     if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) != VK_SUCCESS)
          logger::error("unable to enumerate physical devices");
     
     std::vector<VkPhysicalDevice> devices(device_count);
     if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) != VK_SUCCESS)
          logger::error("unable to enumerate physical devices");

     // what physical device to use (default = 0)
     uint32_t device_index = 0;
     VkPhysicalDeviceProperties2 device_properties = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
     vkGetPhysicalDeviceProperties2(devices[device_index], &device_properties);
     
     logger::info("device name: {}", device_properties.properties.deviceName);
     logger::info("device api version: {}", device_properties.properties.apiVersion);
     logger::info("device driver version: {}", device_properties.properties.driverVersion);
     logger::info("device vendor id: {}", device_properties.properties.vendorID);

     // init queue for graphics queue
     uint32_t queue_family_count = 0;
     vkGetPhysicalDeviceQueueFamilyProperties(devices[device_index], &queue_family_count, nullptr);
     std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
     vkGetPhysicalDeviceQueueFamilyProperties(devices[device_index], &queue_family_count, queue_families.data());

     uint32_t queue_family = 0;
     for (size_t i = 0; i < queue_families.size(); i++)
     {
          if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
          {
               queue_family = i;
               break;
          }
     }

     if (!glfwGetPhysicalDevicePresentationSupport(instance, devices[device_index], queue_family))
          logger::fatal("graphics device does not support graphics queue family");

     logger::trace("initialized vulkan device");
}

App::App(int width, int height, const std::string& title)
     : width(width), height(height), title(title)
{
     if (!glfwInit())
          logger::fatal("unable to initialize glfw");
     glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
     glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

     window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
     if (window == nullptr)
          logger::fatal("unable to initialize window");
     logger::trace("glfw window initialized ({}x{})", width, height);

     // vulkan stuff
     init_validation_layers();
     init_instance();
     init_device();

     logger::debug("initialized vulkan app");
}

App::~App()
{
     glfwDestroyWindow(window);
     glfwTerminate();



     logger::debug("deinitialized vulkan app");
}

void App::run()
{
     double last = glfwGetTime();
     double timer = 0.0f;
     size_t fps_counter = 0;

     while (!glfwWindowShouldClose(window))
     {
          // rendering logic
          glfwSwapBuffers(window);
          fps_counter++;

          // update logic
          glfwPollEvents();
          double now = glfwGetTime();
          dt = now - last;
          last = now;

          timer += dt;
          if (timer >= 1.0f)
          {
               timer = 0.0f;
               logger::info("dt: {:.3f}ms fps: {} ticks: {}", dt * 1e3, fps_counter, ticks);
               fps_counter = 0;
          }

          ticks++;
     }
}