#pragma once

typedef unsigned int GLuint;
typedef signed long long int GLsizeiptr;

class EBO
{
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

