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
#include <Rendering/Material.h>


void InputTick(GLFWwindow* window);
void ExitToDesktop();
void RenderTick();
void PrepareShader(Material* inMaterial, bool randomBogus);
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
Texture testTex3 = Texture();

ShaderDef basicMaterialDef = ShaderDef(
    "BasicFlatLayeredMaterial",
    "game/content/shaders/unlitDefault.vert",
    "game/content/shaders/unlitDefault.frag");
Material* testmaterial;
Material* testmaterial2;
int EngineInit() 
{
    std::string exeLocation = FilesystemExtensions::GetExeLocation();
    std::filesystem::path programRoot = std::filesystem::path(exeLocation).parent_path();
    std::filesystem::current_path(programRoot);

    glfwInit();
    EngineWindow = new VgineWindow();
    GLFWwindow* GLWindow = EngineWindow->GetInternalGLFWwindow();

    testmaterial = new Material(basicMaterialDef, false);
    testmaterial2 = new Material(basicMaterialDef, false);

    PrepareShader(testmaterial, false);
    PrepareShader(testmaterial2, true);
    testmaterial->SetMaterialParam("_time", 0.0f);

    testTex.SetupGLTexture(0);
    testTex2.SetupGLTexture(1);
    testTex3.SetupGLTexture(2);

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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    HandleCameraInput(&cameraTransform);
    testmaterial2->SetMaterialParam("_time", timeValue);

    cubeTransform.SetLocation(glm::vec3(0.0f, 0.0f, 0.0f));

    testmaterial->SetMaterialParam("_model", cubeTransform.GetTransformMatrix());
    testmaterial->SetMaterialParam("_view", awesomeCamera.GetViewMatrix());
    testmaterial->SetMaterialParam("_projection", awesomeCamera.GetProjectionMatrix());

	cubeTransform.SetLocation(glm::vec3(0.0f, 0.0f, 1.0f));

    testmaterial2->SetMaterialParam("_model", cubeTransform.GetTransformMatrix());
    testmaterial2->SetMaterialParam("_view", awesomeCamera.GetViewMatrix());
    testmaterial2->SetMaterialParam("_projection", awesomeCamera.GetProjectionMatrix());


    testmaterial->RenderMaterial();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    testmaterial2->RenderMaterial();
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
    //std::cout << mouseOffsetX << " : " << mouseOffsetY << std::endl;
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

void PrepareShader(Material* inMaterial, bool randomBogus)
{
    inMaterial->InitializeMaterial();

    Basic2DShader = inMaterial->GetActiveShader();

    glUseProgram(Basic2DShader->GetProgramID());

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
    glEnableVertexAttribArray(aUVLocation);

    inMaterial->SetMaterialParam("_OverlayTex", &testTex3);
    if (randomBogus)
        inMaterial->SetMaterialParam("_MainTex", &testTex);
    else
        inMaterial->SetMaterialParam("_MainTex", &testTex2);
}