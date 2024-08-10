#include "atpch.h"
#include "Renderer.h"
#include <GLFW\glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

Renderer::Renderer(uint32_t width, uint32_t height, GLFWwindow* window) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height) , m_Window(window),
VBO1(vertices, sizeof(vertices)), EBO1(indices, sizeof(indices)), whiteSquare("Assets/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE, GL_RGBA, GL_UNSIGNED_BYTE), m_Camera(m_Width, m_Height, glm::vec3(0.0f, 0.0f, 2.0f))
{	
	VAO1 = VAO();
	VAO1.Bind();

	// Initialize VBO1 and EBO1
	VBO1 = VBO(vertices, sizeof(vertices));
	EBO1 = EBO(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	whiteSquare.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer()
{
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	whiteSquare.Delete();
	shaderProgram.Delete();
}

void Renderer::Render()
{
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Tell OpenGL which Shader Program we want to use
	shaderProgram.Activate();

	m_Camera.Inputs(m_Window);
	m_Camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

	whiteSquare.Bind();
	// Bind the VAO so OpenGL knows to use it
	VAO1.Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
}
