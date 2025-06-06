#include "atpch.h"
#include "VertexArray.h""
#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

void VertexArray::LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// m_VertexBuffer.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	// m_VertexBuffer.Unbind();
}

void VertexArray::CreateArrays(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	Bind();

	m_VertexBuffer = VertexBuffer(vertices);
	m_VertexBuffer.Bind();
	m_IndexBuffer = IndexBuffer(indices);
	m_IndexBuffer.Bind();

	LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	LinkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	LinkAttrib(2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	LinkAttrib(3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	Unbind();
	m_VertexBuffer.Unbind();
	m_IndexBuffer.Unbind();

	LOG_DEBUG("Inside CreateArrays - VAO: %u, VBO: %u, EBO: %u",
		ID, m_VertexBuffer.ID, m_IndexBuffer.ID);
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
	LOG_DEBUG("Bound VAO: %d", ID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
