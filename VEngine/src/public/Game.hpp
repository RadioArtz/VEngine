#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Window/VgineWindow.h>
#include <IO/FilesystemExtensions.h>
#include <filesystem>
#include <Scene.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Rendering/ModelLoader.hpp>

namespace VEngine
{
    class Game
    {
    public:

        static Game& Get() 
        {
            static Game game{};
            return game;
        }

        VgineWindow* EngineWindow;
        GLFWwindow* GLWindow;
        float deltaTime;
        float lastTime;    
        bool shouldExit;
        class TestScene* Awesomescene;
        glm::vec3 bgColor = glm::vec3(0.5f, 0.65f, 0.65f);

        int EngineInit();

        void RenderEngineUI();

        void RenderEngineMenuBar();

        VEngine::ModelLoader loader;

        void RenderMaterialEditor(Material* inMaterial);
    };
}
