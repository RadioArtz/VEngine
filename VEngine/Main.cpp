#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Window/VgineWindow.h>

int main() 
{
    glfwInit();
    VgineWindow* EngineWindow = new VgineWindow();
    EngineWindow->NewWindow();

    while (!glfwWindowShouldClose(EngineWindow->GetInternalGLFWwindow()))
    {
        glfwSwapBuffers(EngineWindow->GetInternalGLFWwindow());
        glfwPollEvents();
    }

    return 0;
}