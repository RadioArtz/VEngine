#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window/VgineWindow.h"

VgineWindow::VgineWindow(const char* InWindowTitle, const WindowType InFullscreenMode) { NewWindow(InWindowTitle, InFullscreenMode); }
GLFWwindow* VgineWindow::GetInternalGLFWwindow() { return InternalWindow; }

void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{((VgineWindow*)glfwGetWindowUserPointer(window ))->ResizeFrameBuffer(width,height);}

GLFWwindow* VgineWindow::NewWindow(const char* InWindowTitle, const WindowType InFullscreenMode)
{   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    InternalWindow = glfwCreateWindow(WindowWidth, WindowHeight, InWindowTitle, nullptr, nullptr);
    if (!InternalWindow)
    {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

	glfwSetWindowUserPointer(InternalWindow, this);
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

int VgineWindow::ResizeFrameBuffer(unsigned int Width, unsigned int Height)
{
    glViewport(0, 0, Width, Height);
    std::cout << "Resized Window: " << Width << " " << Height << std::endl;
    return 0;
}