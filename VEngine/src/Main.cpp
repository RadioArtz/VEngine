#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Window/VgineWindow.h>
#include <Rendering/Shader.h>
#include <Rendering/ShaderProgram.h>
#include <IO/FilesystemExtensions.h>
#include <filesystem>
#include <IO/ImageLoader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Transform.hpp>
#include <Camera.hpp>


void InputTick(GLFWwindow* window);
void ExitToDesktop();
void RenderTick();
void PrepareShader();
void SetupTexturesTemp();
void HandleCameraInput(Transform* camTrans);
int EngineInit();
VgineWindow* EngineWindow;
float deltaTime;
float lastTime;
#ifdef _DEBUG
    int main() { return EngineInit(); }
#else
    int WinMain() { return EngineInit(); }
#endif

#pragma region shaders
    /*
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };*/
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
unsigned int myTexture;

int EngineInit() 
{
    std::string exeLocation = FilesystemExtensions::GetExeLocation();
    std::filesystem::path programRoot = std::filesystem::path(exeLocation).parent_path();
    std::filesystem::current_path(programRoot);

    glfwInit();
    EngineWindow = new VgineWindow();
    GLFWwindow* GLWindow = EngineWindow->GetInternalGLFWwindow();
    PrepareShader();

	SetupTexturesTemp();
    glEnable(GL_DEPTH_TEST);
    lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(GLWindow))
    {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        InputTick(GLWindow);
        RenderTick();
        glfwSwapBuffers(GLWindow);
        glfwPollEvents();

    }
    glfwTerminate();
    std::cout << "Closed Engine Window!" << std::endl;
    return 0;
}

// cba to make a texture class rn but i wanna keep EngineInit() a bit clean so yeah
void SetupTexturesTemp() 
{
    int width, height, nrChannels;
    unsigned char* data;
    std::string imagePath = "game/content/textures/test.jpg";
    ImageLoader::LoadImage(imagePath, &data, &width, &height, &nrChannels);

    glGenTextures(1, &myTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
    
void InputTick(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        ExitToDesktop();
}

Transform cameraTransform = Transform();
Transform cubeTransform = Transform();
Camera awesomeCamera = Camera(&cameraTransform);

void RenderTick() 
{
	float timeValue = glfwGetTime();
    if (Basic2DShader->GetProgramID() == -1)
        return;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(Basic2DShader->GetProgramID());

    HandleCameraInput(&cameraTransform);

	int modelLoc = Basic2DShader->GetUniformIndex("_model");
    int viewLoc = Basic2DShader->GetUniformIndex("_view");
    int projectionLoc = Basic2DShader->GetUniformIndex("_projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeTransform.GetTransformMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(awesomeCamera.GetViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(awesomeCamera.GetProjectionMatrix()));

	Basic2DShader->SetShaderValue<float>("_time", timeValue);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	cubeTransform.SetLocation(glm::vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeTransform.GetTransformMatrix()));
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void HandleCameraInput(Transform* camTrans) 
{
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * camTrans->GetForwardVector());
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * -camTrans->GetForwardVector());
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
        camTrans->AddLocation(Transform::NormalizeVector(camTrans->GetRightVector()) * cameraSpeed);
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
        camTrans->AddLocation(Transform::NormalizeVector(-camTrans->GetRightVector()) * cameraSpeed);
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * camTrans->GetUpVector());
    if (glfwGetKey(EngineWindow->GetInternalGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * -camTrans->GetUpVector());
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

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int aPosLocation = Basic2DShader->GetAttributeIndex("aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    //int aColorLocation = Basic2DShader->GetAttributeIndex("aColor");
    //glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(aColorLocation);

	int aUVLocation = Basic2DShader->GetAttributeIndex("aTexCoord");
    glVertexAttribPointer(aUVLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}