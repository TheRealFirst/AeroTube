#pragma once

#include "ShaderClass.h"
#include "Buffers\VAO.h"
#include "Buffers\VBO.h"
#include "Buffers\EBO.h"
#include "../../vendor/stb_image/stb_image.h"


typedef unsigned int GLuint;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();

private:
	Shader shaderProgram;
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	GLuint uniID;
};

