#pragma once
#include <string>
#include <GLFW/glfw3.h>

class App
{

public:
     App(int width, int height, const std::string& title);
     ~App();

     void run();

private:
     GLFWwindow* window;

};