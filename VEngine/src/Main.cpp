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
#include <Rendering/Texture.h>


void InputTick(GLFWwindow* window);
void ExitToDesktop();
void RenderTick();
void PrepareShader();
void SetupTexturesTemp();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
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

double mouseLastX;
double mouseLastY;
float mouseOffsetX;
float mouseOffsetY;

Texture testTex = Texture();
Texture testTex2 = Texture();
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
    testTex.SetupGLTexture(true);
    testTex2.SetupGLTexture(false);
    glEnable(GL_DEPTH_TEST);
    lastTime = glfwGetTime();
    mouseLastX = EngineWindow->GetWindowSize().x / 2.0f;
    mouseLastY = EngineWindow->GetWindowSize().y / 2.0f;
    glfwSetInputMode(GLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(GLWindow, mouse_callback);
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
    //unsigned char* data;
void SetupTexturesTemp() 
{
    /*
    int width, height, nrChannels;
    std::string imagePath = "game/content/textures/test.jpg";
    ImageLoader::LoadImage(imagePath, &data, &width, &height, &nrChannels);

    glGenTextures(1, &myTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    //ImageLoader::FreeImage(data);
    */
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
    cubeTransform.SetLocation(glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeTransform.GetTransformMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(awesomeCamera.GetViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(awesomeCamera.GetProjectionMatrix()));

	//Basic2DShader->SetShaderValue<float>("_time", timeValue);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindTexture(GL_TEXTURE_2D, myTexture);
    testTex.ActivateTexture();

    glDrawArrays(GL_TRIANGLES, 0, 36);
	cubeTransform.SetLocation(glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeTransform.GetTransformMatrix()));
    testTex2.ActivateTexture();
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    mouseOffsetX = xpos - mouseLastX;
    mouseOffsetX *= 0.1f;
    mouseOffsetY = ypos - mouseLastY;   
    mouseOffsetY *= 0.1f;
    mouseLastX = xpos;
    mouseLastY = ypos;
    std::cout << mouseOffsetX << " : " << mouseOffsetY << std::endl;
};
void HandleCameraInput(Transform* camTrans) 
{
    float cameraSpeed = 1.5f * deltaTime;
    GLFWwindow* window = EngineWindow->GetInternalGLFWwindow();
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 5.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * camTrans->GetForwardVector());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * -camTrans->GetForwardVector());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * camTrans->GetRightVector());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * -camTrans->GetRightVector());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * camTrans->GetUpVector());
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camTrans->AddLocation(cameraSpeed * -camTrans->GetUpVector());
        

    camTrans->AddRotation(-mouseOffsetY, mouseOffsetX, 0);
    mouseOffsetX = 0;
    mouseOffsetY = 0;
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