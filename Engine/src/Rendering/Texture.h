#pragma once

#include "ShaderClass.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;

	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void UnBind();
	void Delete();
};

