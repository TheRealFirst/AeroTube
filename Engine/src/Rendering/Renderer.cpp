#include "atpch.h"
#include "Renderer.h"
#include <GLFW\glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


Renderer::Renderer(uint32_t width, uint32_t height, GLFWwindow* window) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height) , m_Window(window),
VBO1(vertices, sizeof(vertices)), EBO1(indices, sizeof(indices)), texture1("Assets/Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE), specular1("Assets/Textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE), m_Camera(m_Width, m_Height, glm::vec3(0.0f, 0.0f, 2.0f)), lightShader("Assets/Shaders/light.vert", "Assets/Shaders/light.frag"), lightVBO(lightVertices, sizeof(lightVertices)), lightEBO(lightIndices, sizeof(lightIndices))
{	
	VAO1 = VAO();
	VAO1.Bind();

	// Initialize VBO1 and EBO1
	VBO1 = VBO(vertices, sizeof(vertices));
	EBO1 = EBO(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	texture1.texUnit(shaderProgram, "tex0", 0);
	specular1.texUnit(shaderProgram, "tex1", 1);

	lightVAO = VAO();
	lightVAO.Bind();

	lightVBO = VBO(lightVertices, sizeof(lightVertices));
	lightEBO = EBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer()
{
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture1.Delete();
	specular1.Delete();
	shaderProgram.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
}

void Renderer::Render(float deltaTime)
{
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Tell OpenGL which Shader Program we want to use

	m_Camera.Inputs(m_Window, deltaTime);
	m_Camera.updateMatrix(45.0f, 0.1f, 100.0f);

	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);
	m_Camera.Matrix(shaderProgram, "camMatrix");

	texture1.Bind();
	specular1.Bind();
	// Bind the VAO so OpenGL knows to use it
	VAO1.Bind();
	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

	lightShader.Activate();
	m_Camera.Matrix(lightShader, "camMatrix");
	lightVAO.Bind();
	glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
}

void Renderer::Resize(uint32_t newWidth, uint32_t newHeight)
{
	m_Width = newWidth;
	m_Height = newHeight;

	m_Camera.Resize(m_Width, m_Height);
}
