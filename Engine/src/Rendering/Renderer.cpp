#include "atpch.h"
#include "Renderer.h"
#include <GLFW\glfw3.h>
#include <glad/glad.h>

// Vertices coordinates
GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS           //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

Renderer::Renderer() : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	VBO1(vertices, sizeof(vertices)), EBO1(indices, sizeof(indices))
{	
	VAO1 = VAO();
	VAO1.Bind();

	// Initialize VBO1 and EBO1
	VBO1 = VBO(vertices, sizeof(vertices));
	EBO1 = EBO(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	uniID = glGetUniformLocation(shaderProgram.ID, "scale");
}

Renderer::~Renderer()
{
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
}

void Renderer::Render()
{
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Tell OpenGL which Shader Program we want to use
	shaderProgram.Activate();
	glUniform1f(uniID, 0.5f);
	// Bind the VAO so OpenGL knows to use it
	VAO1.Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
}
