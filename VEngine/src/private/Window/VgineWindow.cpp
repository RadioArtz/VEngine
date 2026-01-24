#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window/VgineWindow.h"

VgineWindow::VgineWindow() { }

void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "Resized Window: " << width << " " << height << std::endl;
}

GLFWwindow* VgineWindow::GetInternalGLFWwindow() { return InternalWindow; }

GLFWwindow* VgineWindow::NewWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    InternalWindow = glfwCreateWindow(WindowWidth, WindowHeight, "Enjin2", nullptr, nullptr);
    if (!InternalWindow)
    {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(InternalWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad failed to initialize" << std::endl;
        return nullptr;
    }

    glViewport(0, 0, WindowWidth, WindowHeight);
    glfwSetFramebufferSizeCallback(InternalWindow, Framebuffer_size_callback);

    return InternalWindow;
}