#include <string>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "app.h"
#include "../util/log.h"

App::App(int width, int height, const std::string& title)
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
     VkApplicationInfo app_info = {
          .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
          .pApplicationName = title.c_str(),
          .apiVersion = VK_API_VERSION_1_3
     };

     std::uint32_t count = 0;
     const char** extensions = glfwGetRequiredInstanceExtensions(&count);
     logger::trace("found {} instance extensions:", count);
     for (int i = 0; i < count; i++)
          logger::trace("  [{}] = {}", i, extensions[i]);

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