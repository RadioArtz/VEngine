#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Window/VgineWindow.h>
#include <Rendering/Shader.h>


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

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

unsigned int shaderProgram;
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

int EngineInit() 
{
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
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
    VertexShader.SetShaderSource("content/shaders/unlitDefault.vert", EShaderType::Vertex);
    VertexShader.CompileShader();

    Shader FragmentShader = Shader();
    FragmentShader.SetShaderSource("content/shaders/unlitDefault.frag", EShaderType::Fragment);
    FragmentShader.CompileShader();

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VertexShader.GetShaderID());
    glAttachShader(shaderProgram, FragmentShader.GetShaderID());
    glLinkProgram(shaderProgram);

    glDeleteShader(VertexShader.GetShaderID());
    glDeleteShader(FragmentShader.GetShaderID());

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