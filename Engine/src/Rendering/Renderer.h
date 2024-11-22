#pragma once

#include "ShaderClass.h"
#include "Buffers\VAO.h"
#include "Buffers\VBO.h"
#include "Buffers\EBO.h"
#include "Texture.h"
#include "glm\glm.hpp"

#include "Camera.h"

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
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	Texture texture1;
	Texture specular1;

	Shader lightShader;
	VAO lightVAO;
	VBO lightVBO;
	EBO lightEBO;
	

	Camera m_Camera;
};

