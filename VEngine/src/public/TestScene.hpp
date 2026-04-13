#pragma once
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
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Game.hpp>

namespace VEngine 
{
    class TestScene : public Scene 
    {
    public: 
        using Scene::Scene;
        VgineWindow* EngineWindow;

        double mouseX;
        double mouseY;
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
        Transform objTransform = Transform();
        Camera awesomeCamera = Camera(&cameraTransform);
        ModelLoader loader;
        PointLight testLight;

        float rotationRate = 1;
    
    
        GameObject johnTester = GameObject{ "Test GameObject" };

        void OnBeginPlay() 
        {
            testmaterial = new Material(basicLitDef, false);

            PrepareShader(testmaterial, false);
            testmaterial->SetMaterialParam("_time", 0.0f);

            albedoTexture = Texture("C:/Users/mathi/Downloads/viking_room.png");
            albedoTexture.SetupGLTexture();

            testmaterial->SetMaterialParam("_MainTex", &albedoTexture);
            testmaterial->SetMaterialParam("_EmissiveTex", &emissiveTexture);
            testmaterial->SetMaterialParam("lightPosition", cameraTransform.GetLocation());

            objTransform.SetRotation(-90, 0, 180);

            mouseLastX = EngineWindow->GetWindowSize().x / 2.0f;
            mouseLastY = EngineWindow->GetWindowSize().y / 2.0f;
            glfwSetCursorPos(EngineWindow->GetInternalGLFWwindow(), mouseLastX, mouseLastY);
        }

        void OnUpdate(float deltaTime)
        {
            RenderUI();
            InputTick(EngineWindow->GetInternalGLFWwindow(), deltaTime);
        }

        void OnRender(float deltaTime) 
        {
            float timeValue = glfwGetTime();
        
            rotationRate += deltaTime * 1.0f;
            objTransform.AddRotation(0, 0, deltaTime * 30.0f);

            //testmaterial->SetMaterialParam("_time", timeValue);
            testLight.specularColor.Intensity = 5.0f;

            testmaterial->SetMaterialParam("_ambientColor", Game::Get().bgColor);
            testmaterial->SetMaterialParam("diffuseColor", glm::vec3(1));
            testmaterial->SetMaterialParam("diffuseLightColor", (testLight.diffuseColor.Color * testLight.diffuseColor.Intensity));

            testmaterial->SetMaterialParam("specularHardness", testLight.specularHardness);
            testmaterial->SetMaterialParam("specularLightColor", (testLight.specularColor.Color * testLight.specularColor.Intensity));

            testmaterial->SetMaterialParam("_viewPos", cameraTransform.GetLocation());
            testmaterial->SetMaterialParam("_model", objTransform.GetTransformMatrix());
            testmaterial->SetMaterialParam("_view", awesomeCamera.GetViewMatrix());
            testmaterial->SetMaterialParam("_projection", awesomeCamera.GetProjectionMatrix());

            testmaterial->RenderMaterial();
            if (!loader.meshes.empty())
                loader.meshes[0].Draw();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        float floats[3]{ 0,0,0 };
        void RenderImportDialog()
        {
            bool pressed = false;
            ImGui::Begin("Import Model");
            ImGui::SetWindowSize(ImVec2(250, 400));
            std::string input = "C:/Users/mathi/Downloads/viking_room.obj";
            if (ImGui::Button("Import"))
            {
                pressed = true;
            }
            ImGui::SameLine();
            ImGui::InputText("Model Path", &input);
            if (pressed)
                loader.loadModel(input);

            if (!loader.meshes.empty())
            {
                ImGui::DragFloat3("Location", floats);
                std::cout << floats[0] << floats[1] << floats[2] << std::endl;
                objTransform.SetLocation(floats[0], floats[1], floats[2]);
            }


            ImGui::End();
        };

        bool toolboxOpen = true;
        bool ImportDialogOpen = false;
        void RenderUI()
        {
            ImGui::Begin("Toolbox", &toolboxOpen, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::SetWindowSize(ImVec2(350, 100));

            if (ImGui::Button("Import", ImVec2(70, 70)))
            {
                ImportDialogOpen = !ImportDialogOpen;
                std::cout << ImportDialogOpen << std::endl;
            }
            ImGui::End();

            if (ImportDialogOpen)
                RenderImportDialog();
            RenderSceneHierarchy();
        }

        void InputTick(GLFWwindow* window, float deltaTime)
        {
            UpdateCursorPos(window);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                ExitToDesktop();

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            {
                HandleCameraInput(&cameraTransform, deltaTime);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void UpdateCursorPos(GLFWwindow* window)
        {
            glfwGetCursorPos(window, &mouseX, &mouseY);

            mouseOffsetX = mouseX - mouseLastX;
            mouseOffsetX *= 0.1f;
            mouseOffsetY = mouseY - mouseLastY;
            mouseOffsetY *= 0.1f;

            mouseLastX = mouseX;
            mouseLastY = mouseY;
            //std::cout << mouseOffsetX << " : " << mouseOffsetY << std::endl;
        }

        void HandleCameraInput(Transform* camTrans, float deltaTime)
        {
            float cameraSpeed = 1.5f * deltaTime;
            GLFWwindow* window = EngineWindow->GetInternalGLFWwindow();
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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

        void RenderSceneHierarchy()
        {
            ImGui::Begin("Scene Hierarchy");
            ImGui::SetWindowSize(ImVec2(300, 500));
            for (Scene* scn : Scene::activeScenes)
            {
                if (ImGui::CollapsingHeader(scn->sceneName.c_str())) 
                {
                    for (GameObject* go : scn->gameObjects) 
                    {
                        ImGui::PushID(go);

                        if (ImGui::TreeNode(go->objectName.c_str())) 
                        {
                            ImGui::Button("aga");
                            ImGui::TreePop();
                        }
                        ImGui::PopID();
                    }
                }
            }
            ImGui::End();
        }
    };
}