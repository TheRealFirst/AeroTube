#pragma once

typedef unsigned int GLuint;
typedef signed long long int GLsizeiptr;
typedef unsigned int GLenum;
#include "VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

