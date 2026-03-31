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
#include <Rendering/Mesh.h>
#include <Rendering/ModelLoader.hpp>
#include <Rendering/Light.h>


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

double mouseLastX;
double mouseLastY;
float mouseOffsetX;
float mouseOffsetY;

//Texture testTex = Texture("C:/Users/mathi/Downloads/viking_room.png");
Texture albedoTexture;
Texture emissiveTexture;

ShaderDef basicMaterialDef = ShaderDef(
    "BasicFlatMaterial",
    "game/content/shaders/unlitDefault.vert",
    "game/content/shaders/unlitDefault.frag");
ShaderDef basicLitDef = ShaderDef(
    "BasicLitMaterial",
    "game/content/shaders/unlitDefault.vert",
    "game/content/shaders/litDefault.frag");

Material* testmaterial;

Transform cameraTransform = Transform();
Transform cubeTransform = Transform();
Camera awesomeCamera = Camera(&cameraTransform);
ModelLoader loader;
PointLight testLight;

int EngineInit() 
{
    std::string exeLocation = FilesystemExtensions::GetExeLocation();
    std::filesystem::path programRoot = std::filesystem::path(exeLocation).parent_path();
    std::filesystem::current_path(programRoot);

    glfwInit();
    EngineWindow = new VgineWindow();
    GLFWwindow* GLWindow = EngineWindow->GetInternalGLFWwindow();

    testmaterial = new Material(basicLitDef, false);

    PrepareShader(testmaterial, false);
    testmaterial->SetMaterialParam("_time", 0.0f);

    albedoTexture = Texture("C:/Users/mathi/Documents/Atlas_Albedo_8k.png");
    albedoTexture.SetupGLTexture();
    emissiveTexture = Texture("C:/Users/mathi/Documents/Atlas_Emissive.png");
    emissiveTexture.SetupGLTexture();

    testmaterial->SetMaterialParam("_MainTex", &albedoTexture);
    testmaterial->SetMaterialParam("_EmissiveTex", &emissiveTexture);
    testmaterial->SetMaterialParam("lightPosition", cameraTransform.GetLocation());

    loader.loadModel("C:/Users/mathi/Documents/Kerie Opt.fbx");

    cubeTransform.SetRotation(-90, 0, 180);
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
    HandleCameraInput(&cameraTransform);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        ExitToDesktop();

}
float rotationRate = 1;
    glm::vec3 bgColor(0.2f, 0.3f, 0.3f);
void RenderTick() 
{
	float timeValue = glfwGetTime();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rotationRate += deltaTime * 1.0f;
    cubeTransform.AddRotation(0 , 0 , deltaTime * 30.0f);
    //testmaterial->SetMaterialParam("_time", timeValue);

    testmaterial->SetMaterialParam("_ambientColor", bgColor);
    testmaterial->SetMaterialParam("diffuseColor", glm::vec3(1));
    testmaterial->SetMaterialParam("diffuseLightColor", (testLight.diffuseColor.Color * testLight.diffuseColor.Intensity));

    testmaterial->SetMaterialParam("specularHardness", testLight.specularHardness);
    testmaterial->SetMaterialParam("specularLightColor", (testLight.specularColor.Color * testLight.specularColor.Intensity));

    testmaterial->SetMaterialParam("_viewPos", cameraTransform.GetLocation());
    testmaterial->SetMaterialParam("_model", cubeTransform.GetTransformMatrix());
    testmaterial->SetMaterialParam("_view", awesomeCamera.GetViewMatrix());
    testmaterial->SetMaterialParam("_projection", awesomeCamera.GetProjectionMatrix());

    testmaterial->RenderMaterial();
    loader.meshes[0].Draw();

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
}

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
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        testmaterial->SetMaterialParam("lightPosition", cameraTransform.GetLocation());

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
    glUseProgram(inMaterial->GetActiveShader()->GetProgramID());
}