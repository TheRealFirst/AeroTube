#pragma once

#include "ShaderClass.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void UnBind();
	void Delete();
};

