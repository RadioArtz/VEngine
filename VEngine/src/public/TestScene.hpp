#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Window/VgineWindow.h>
#include <Transform.hpp>
#include <Camera.hpp>
#include <Rendering/Texture.h>
#include <Rendering/Material.h>
#include <Rendering/Light.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <Game.hpp>
#include <GameObject.h>
#include <Tests/LifecycleTestComponent.h>
#include <Rendering/StaticMeshComponent.h>
#include <FreeCamControler.hpp>

namespace VEngine 
{
    class TestScene : public Scene 
    {
    public: 
        using Scene::Scene;
        VgineWindow* EngineWindow;



        Texture albedoTexture;
        Texture emissiveTexture;

        ShaderDef basicLitDef = ShaderDef(
            "BasicLitMaterial",
            "game/content/shaders/unlitDefault.vert",
            "game/content/shaders/litDefault.frag");

        std::shared_ptr<Material> testmaterial;

        GameObject cameraObject = GameObject("CamHolder");
        //Transform cameraTransform = Transform();
        //Camera awesomeCamera = Camera(&cameraTransform);
        PointLight testLight;
        
        GameObject FreeCamObject = GameObject("FreeCam");
        GameObject testObject = GameObject{ "Test GameObject" };
        GameObject testChild = GameObject{ "Test ChildNode" };

        void OnBeginPlay() 
        {
            cameraObject.AddComponent<Camera>();
            cameraObject.AddComponent<FreeCamControler>();

            testChild.SetParent(&testObject);
            testmaterial = std::make_shared<Material>(basicLitDef, false);

            PrepareShader(testmaterial, false);
            testmaterial->SetMaterialParam("_time", 0.0f);

            albedoTexture = Texture("C:/Users/mathi/Downloads/viking_room.png");
            albedoTexture.SetupGLTexture();

            testmaterial->SetMaterialParam("_MainTex", &albedoTexture);
            testmaterial->SetMaterialParam("_EmissiveTex", &emissiveTexture);
            testmaterial->SetMaterialParam("lightPosition", cameraObject.transform.GetLocation());

            testObject.transform.SetRelativeRotation(-90, 0, 180);
            testObject.AddComponent<LifecycleTestComponent>();

            Scene::OnBeginPlay();
        }

        bool meshApplied = false;;
        void OnUpdate(double deltaTime)
        {
            Scene::OnUpdate(deltaTime);
            RenderUI();

            if (Game::Get().loader.meshes.size() != 0 && !meshApplied) 
            {
                meshApplied = true;
                testObject.AddComponent<StaticMeshComponent>(Game::Get().loader.meshes[0], testmaterial);
            }
                

            InputTick(EngineWindow->GetInternalGLFWwindow(), deltaTime);
        }

        void OnRender(float deltaTime) 
        {
            float timeValue = glfwGetTime();
        
            testLight.specularColor.Intensity = 5.0f;

            testmaterial->SetMaterialParam("_ambientColor", Game::Get().bgColor);
            testmaterial->SetMaterialParam("diffuseColor", glm::vec3(1));
            testmaterial->SetMaterialParam("diffuseLightColor", (testLight.diffuseColor.Color * testLight.diffuseColor.Intensity));

            testmaterial->SetMaterialParam("specularHardness", testLight.specularHardness);
            testmaterial->SetMaterialParam("specularLightColor", (testLight.specularColor.Color * testLight.specularColor.Intensity));

            testmaterial->SetMaterialParam("_viewPos", cameraObject.transform.GetLocation());
            testmaterial->SetMaterialParam("_view", cameraObject.GetComponent<Camera>()->GetViewMatrix());
            testmaterial->SetMaterialParam("_projection", cameraObject.GetComponent<Camera>()->GetProjectionMatrix());


                
            Scene::OnRender(deltaTime);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }



        void RenderUI()
        {
        }

        void InputTick(GLFWwindow* window, float deltaTime)
        {

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                ExitToDesktop();

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            {
                //HandleCameraInput(&cameraTransform, deltaTime);
                cameraObject.GetComponent<FreeCamControler>()->HandleCameraInput(deltaTime);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void ExitToDesktop()
        {
            std::cout << "Preparing to Exit..." << std::endl;
            glfwSetWindowShouldClose(EngineWindow->GetInternalGLFWwindow(), true);
        }

        void PrepareShader(std::shared_ptr<Material> inMaterial, bool randomBogus)
        {
            inMaterial->InitializeMaterial();
            glUseProgram(inMaterial->GetActiveShader()->GetProgramID());
        }
    };
}