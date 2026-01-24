#pragma once
#include <GLFW/glfw3.h>

class VgineWindow
{
public:
	VgineWindow();
	GLFWwindow* NewWindow();
	GLFWwindow* GetInternalGLFWwindow();
private:
	GLFWwindow* InternalWindow;
	int WindowWidth = 1280;
	int WindowHeight = 720;
};