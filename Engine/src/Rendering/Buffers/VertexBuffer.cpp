#include "atpch.h"
#include"VertexBuffer.h"
#include <glad\glad.h>

namespace Engine {
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VertexBuffer::VertexBuffer(std::vector <Vertex>& vertices)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	}

	// Binds the VBO
	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	// Unbinds the VBO
	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Deletes the VBO
	void VertexBuffer::Delete()
	{
		glDeleteBuffers(1, &m_ID);
	}
}