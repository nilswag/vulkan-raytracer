#include <string>
#include <GLFW/glfw3.h>
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
     while (!glfwWindowShouldClose(window))
     {
          glfwPollEvents();
     }
}