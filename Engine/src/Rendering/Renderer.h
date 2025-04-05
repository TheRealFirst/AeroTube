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
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void Clear(const glm::vec4& color);

	void Render(float deltaTime, const Camera& camera);

	

	void OnWindowResize(uint32_t newWidth, uint32_t newHeight);
private:
	uint32_t m_Width;
	uint32_t m_Height;

	Shader shaderProgram;
	Model model;

	Scene* m_TestScene; // TODO: Move to Editorlayer
};

