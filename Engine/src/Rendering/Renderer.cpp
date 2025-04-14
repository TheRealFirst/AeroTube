#include "atpch.h"

#include "Renderer.h"
#include "glad\glad.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

Renderer::Renderer(uint32_t width, uint32_t height) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height)
{
	// Initialize the model with the default bunny model using the model cache
	model = Model::LoadModel("Assets/Models/bunny/scene.gltf");

	// Use the member variables for light properties
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, m_LightPosition);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);

	glEnable(GL_DEPTH_TEST);

	// Disable face culling for now to ensure all models render correctly
	glDisable(GL_CULL_FACE);
	// When face culling is enabled, use these settings:
	// glCullFace(GL_BACK);
	// glFrontFace(GL_CCW);


	m_TestScene = new Scene();
}

Renderer::~Renderer()
{
	shaderProgram.Delete();
	delete model;
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

	// Update light properties
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);

	// Create model matrix from transformation properties
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// Apply transformations in the correct order: scale, rotate, translate
	modelMatrix = glm::translate(modelMatrix, m_ModelPosition);

	// Apply rotation around each axis
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = glm::scale(modelMatrix, m_ModelScale);

	// Set the model matrix uniform
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	camera.Matrix(shaderProgram, "camMatrix");
	model->Draw(shaderProgram, camera);

	m_TestScene->DrawScene(camera.Position, camera.Orientation, camera.Up, m_Width, m_Height);
}

void Renderer::OnWindowResize(uint32_t newWidth, uint32_t newHeight)
{
	m_Width = newWidth;
	m_Height = newHeight;
}

void Renderer::SetLightPosition(const glm::vec3& position)
{
	m_LightPosition = position;

	// Update the light position in the shader
	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);
}

void Renderer::SetLightColor(const glm::vec4& color)
{
	m_LightColor = color;

	// Update the light color in the shader
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w);
}

void Renderer::LoadModel(const std::string& modelPath)
{
	// No need to delete the old model since it's cached
	// Just load the new model from cache or create it if not cached
	model = Model::LoadModel(modelPath.c_str());

	// Log the model loading
	LOG_INFO("Switched to model: {0}", modelPath);
}

