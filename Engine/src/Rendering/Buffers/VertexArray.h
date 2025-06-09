#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

typedef unsigned int GLuint;
typedef signed long long int GLsizeiptr;
typedef unsigned int GLenum;

class VertexArray
{
public:
	GLuint ID;
	VertexArray();
	~VertexArray() = default;

	void CreateArrays(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void Bind();
	void Unbind();

	void Delete();

private:
	void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};

