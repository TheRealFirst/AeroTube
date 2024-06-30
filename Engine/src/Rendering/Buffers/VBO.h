#pragma once

typedef unsigned int GLuint;
typedef float GLfloat;
typedef signed long long int GLsizeiptr;


class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

