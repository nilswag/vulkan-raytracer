#include <string>
#include <cstdint>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "app.h"
#include "../util/log.h"

void App::init_instance()
{
     VkApplicationInfo app_info = {
          .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
          .pApplicationName = title.c_str(),
          .apiVersion = VK_API_VERSION_1_3
     };

     uint32_t extension_count = 0;
     const char** extensions = glfwGetRequiredInstanceExtensions(&extension_count);
     logger::trace("found {} instance extensions:", extension_count);
     for (int i = 0; i < extension_count; i++)
          logger::trace("  [{}] = {}", i, extensions[i]);

     VkInstanceCreateInfo instance_ci = {
          .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
          .pApplicationInfo = &app_info,
          .enabledExtensionCount = extension_count,
          .ppEnabledExtensionNames = extensions
     };

     if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
          logger::fatal("unable to initialize vulkan instance");
     logger::trace("initialized vulkan instance");
}

void App::init_device()
{
     uint32_t device_count = 0;
     if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) != VK_SUCCESS)
          logger::fatal("unable to enumerate physical devices");
     
     std::vector<VkPhysicalDevice> devices(device_count);
     if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) != VK_SUCCESS)
          logger::fatal("unable to enumerate physical devices");

     // what physical device to use (default = 0)
     uint32_t device_index = 0;
     VkPhysicalDeviceProperties2 device_properties = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
     vkGetPhysicalDeviceProperties2(devices[device_index], &device_properties);
     logger::debug("device name: {}", device_properties.properties.deviceName);
     logger::debug("device api version: {}", device_properties.properties.apiVersion);
     logger::debug("device driver version: {}", device_properties.properties.driverVersion);
     logger::debug("device vendor id: {}", device_properties.properties.vendorID);

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
               logger::debug("dt: {:.3f}ms\t fps: {}\t ticks: {}", dt * 1e3, fps_counter, ticks);
               fps_counter = 0;
          }

          ticks++;
     }
}