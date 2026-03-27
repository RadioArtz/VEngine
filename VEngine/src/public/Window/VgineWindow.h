#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class VgineWindow
{
public:
	enum class WindowType
	{
		Windowed = 0,
		Fullscreen = 1,
		BorderlessFullscreen = 2,
		BorderlessWindowed = 3
	};

public:
	VgineWindow(const char* InWindowTitle = "Vgine Game", const WindowType InFullscreenMode = WindowType::Windowed);
	GLFWwindow* NewWindow(const char* InWindowTitle, const WindowType InFullscreenMode);
	GLFWwindow* GetInternalGLFWwindow();
	int ResizeFrameBuffer(unsigned int Width, unsigned int Height);
	// FIXME: Returning a const value rarely makes sense. You can remove the const from the vector and add it to the function here!
	const glm::vec2 GetWindowSize() { return glm::vec2(WindowWidth, WindowHeight); };

private:
	GLFWwindow* InternalWindow;	// Also initialize this with nullptr
	int WindowWidth = 1280;
	int WindowHeight = 720;
	bool MaintainAspectRatio = false;
	WindowType FullscreenMode = WindowType::Windowed;


};