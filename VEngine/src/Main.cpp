#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Window/VgineWindow.h>
#include <Rendering/Shader.h>
#include <Rendering/ShaderProgram.h>
#include "IO/FilesystemExtensions.h"
#include <filesystem>


void InputTick(GLFWwindow* window);
void ExitToDesktop();
void RenderTick();
void PrepareShader();
int EngineInit();
VgineWindow* EngineWindow;

#ifdef _DEBUG
    int main() { return EngineInit(); }
#else
    int WinMain() { return EngineInit(); }
#endif

#pragma region shaders

float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
#pragma endregion

ShaderProgram* Basic2DShader;

int EngineInit() 
{
    std::string exeLocation = FilesystemExtensions::GetExeLocation();
    std::filesystem::path programRoot = std::filesystem::path(exeLocation).parent_path();
    std::filesystem::current_path(programRoot);

    glfwInit();
    EngineWindow = new VgineWindow();
    GLFWwindow* GLWindow = EngineWindow->GetInternalGLFWwindow();
    PrepareShader();
    while (!glfwWindowShouldClose(GLWindow))
    {
        InputTick(GLWindow);
        RenderTick();
        glfwSwapBuffers(GLWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    std::cout << "Closed Engine Window!" << std::endl;
    return 0;
}

void InputTick(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        ExitToDesktop();
}

void RenderTick() 
{
	float timeValue = glfwGetTime();
    if(Basic2DShader->GetProgramID() != -1)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(Basic2DShader->GetProgramID());
	Basic2DShader->SetShaderValue<float>("_time", timeValue);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ExitToDesktop() 
{
    std::cout << "Preparing to Exit..." << std::endl;
    glfwSetWindowShouldClose(EngineWindow->GetInternalGLFWwindow(), true);
}

void PrepareShader() 
{
    Shader VertexShader = Shader();
    VertexShader.SetShaderSource("game/content/shaders/unlitDefault.vert", EShaderType::Vertex);
    VertexShader.CompileShader();

    Shader FragmentShader = Shader();
    FragmentShader.SetShaderSource("game/content/shaders/unlitDefault.frag", EShaderType::Fragment);
    FragmentShader.CompileShader();

    Basic2DShader = new ShaderProgram();
    Basic2DShader->AttachShader(VertexShader.GetShaderID());
    Basic2DShader->AttachShader(FragmentShader.GetShaderID());
    Basic2DShader->LinkProgram();

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}