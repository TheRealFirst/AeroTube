#include "atpch.h"
#include"IndexBuffer.h"
#include <glad\glad.h>

// Constructor that generates a Elements Buffer Object and links it to indices
IndexBuffer::IndexBuffer(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

// Binds the EBO
void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void IndexBuffer::Delete()
{
	glDeleteBuffers(1, &ID);
}
