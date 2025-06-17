#include "atpch.h"
#include"IndexBuffer.h"
#include <glad\glad.h>

namespace Engine {
	// Constructor that generates a Elements Buffer Object and links it to indices
	IndexBuffer::IndexBuffer(std::vector<uint32_t>& indices)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
	}

	// Binds the EBO
	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	// Unbinds the EBO
	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Deletes the EBO
	void IndexBuffer::Delete()
	{
		glDeleteBuffers(1, &m_ID);
	}
}