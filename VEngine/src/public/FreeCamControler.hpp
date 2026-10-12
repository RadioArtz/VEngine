#pragma once
#include <Component.hpp>
#include <Transform.hpp>
#include <GameObject.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Game.hpp>

namespace VEngine
{
	class FreeCamControler : public Component
	{
	private:
        Transform* camTrans;
        GLFWwindow* window;

        double mouseX;
        double mouseY;
        double mouseLastX;
        double mouseLastY;
        float mouseOffsetX;
        float mouseOffsetY;

	public:
        FreeCamControler() 
        { 
            //window = Game::Get().EngineWindow->GetInternalGLFWwindow();
            //mouseX = mouseLastX = Game::Get().EngineWindow->GetWindowSize().x / 2.0f;
            //mouseY = mouseLastY = Game::Get().EngineWindow->GetWindowSize().y / 2.0f;
            //glfwSetCursorPos(Game::Get().EngineWindow->GetInternalGLFWwindow(), mouseLastX, mouseLastY);
            
        };
        void OnAwake() override 
        {
            camTrans = &owner->transform;
            window = Game::Get().GLWindow;
            mouseX = mouseLastX = Game::Get().EngineWindow->GetWindowSize().x / 2.0f;
            mouseY = mouseLastY = Game::Get().EngineWindow->GetWindowSize().y / 2.0f;
            glfwSetCursorPos(Game::Get().EngineWindow->GetInternalGLFWwindow(), mouseLastX, mouseLastY);
        }

        void OnUpdate(double deltaTime) override
        {
            UpdateCursorPos();
        };

        void UpdateCursorPos()
        {
            glfwGetCursorPos(window, &mouseX, &mouseY);

            mouseOffsetX = mouseX - mouseLastX;
            mouseOffsetX *= 0.1f;
            mouseOffsetY = mouseY - mouseLastY;
            mouseOffsetY *= 0.1f;

            mouseLastX = mouseX;
            mouseLastY = mouseY;
        }

        void HandleCameraInput(float deltaTime)
        {

            float cameraSpeed = 1.5f * deltaTime;
            
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraSpeed = 5.0f * deltaTime;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * camTrans->GetForwardVector());
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * -camTrans->GetForwardVector());
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * -camTrans->GetRightVector());
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * camTrans->GetRightVector());
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * camTrans->GetUpVector());
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                camTrans->AddRelativeLocation(cameraSpeed * -camTrans->GetUpVector());
            //if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                //testmaterial->SetMaterialParam("lightPosition", camTrans.GetLocation());

            camTrans->AddRelativeRotation(mouseOffsetY, -mouseOffsetX, 0);
            mouseOffsetX = 0;
            mouseOffsetY = 0;
        }
	};
}