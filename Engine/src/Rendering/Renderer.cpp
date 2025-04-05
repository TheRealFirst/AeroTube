#include "atpch.h"



#include "Renderer.h"
#include "glad\glad.h"
#include "stb_image.h"

#include "imgui.h"

Renderer::Renderer(uint32_t width, uint32_t height) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height), model("Assets/Models/bunny/scene.gltf")
{	
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	

	m_TestScene = new Scene();
}

Renderer::~Renderer()
{
	shaderProgram.Delete();
	delete m_TestScene;
	
}

void Renderer::Clear(const glm::vec4& color)
{
	// Specify the color of the background
	glClearColor(color.r, color.g, color.b, color.a);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(float deltaTime, const Camera& camera)
{
	// Tell OpenGL which Shader Program we want to use
	shaderProgram.Activate();

	camera.Matrix(shaderProgram, "camMatrix");
	model.Draw(shaderProgram, camera);

	m_TestScene->DrawScene(camera.Position, camera.Orientation, camera.Up, m_Width, m_Height);
}

void Renderer::OnWindowResize(uint32_t newWidth, uint32_t newHeight)
{
	m_Width = newWidth;
	m_Height = newHeight;
}

