#pragma once


typedef unsigned int GLuint;


#include "vector"

class EBO
{
public:
	GLuint ID;
	EBO() = default;
	EBO(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

