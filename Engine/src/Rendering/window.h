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
	void OnResize(int width, int height);
private:
	//GLFWwindow* m_Window;
	uint32_t m_Width;
	uint32_t m_Height;
	float aspectRatio;
	Renderer* m_Renderer;

	float lastFrameTime;

	int newFrameWidth, newFrameHeight;

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
};

