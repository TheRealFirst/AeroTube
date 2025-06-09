#pragma once

#include "glm\glm.hpp"
#include "vector"

typedef unsigned int GLuint;


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};


class VertexBuffer
{
public:
	GLuint ID = 0;
	VertexBuffer() = default;
	VertexBuffer(std::vector <Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

