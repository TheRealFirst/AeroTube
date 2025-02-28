#pragma once

#include "ShaderClass.h"
#include "Buffers\VAO.h"
#include "Buffers\VBO.h"
#include "Buffers\EBO.h"
#include "Texture.h"
#include "glm\glm.hpp"

#include "Camera.h"
#include "Model.h"
#include "Scene\Scene.h"
#include "ImGui/ImGuiLayer.h"


class Renderer
{
public:
	Renderer(uint32_t width, uint32_t height, GLFWwindow* window);
	~Renderer();

	void Render(float deltaTime);

	void Resize(uint32_t newWidth, uint32_t newHeight);
private:
	uint32_t m_Width;
	uint32_t m_Height;
	GLFWwindow* m_Window;

	Shader shaderProgram;
	Model model;

	Camera m_Camera;

	Scene* m_TestScene;

	std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
};

