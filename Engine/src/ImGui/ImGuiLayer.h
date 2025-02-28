#pragma once


struct GLFWwindow;

class ImGuiLayer
{
public:
	ImGuiLayer() = default;
	~ImGuiLayer();

	void InitializeImGui(GLFWwindow* window);
	void ImGuiNewFrame();
	void ImGuiRender();
};

