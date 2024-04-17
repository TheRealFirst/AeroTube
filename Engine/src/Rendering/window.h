#pragma once
#include <stdint.h>
#include "Renderer.h"

class Window
{
public:
	Window(uint32_t width, uint32_t height);
	~Window();
	void Run();
	bool isRunning();

private:
	//GLFWwindow* m_Window;
	uint32_t m_Width = 800;
	uint32_t m_Height = 800;
	float aspectRatio;
	Renderer* m_Renderer;
};

