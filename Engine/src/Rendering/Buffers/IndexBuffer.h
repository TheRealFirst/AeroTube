#pragma once


typedef unsigned int GLuint;


#include "vector"

class IndexBuffer
{
public:
	GLuint ID = 0;
	IndexBuffer() = default;
	IndexBuffer(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

