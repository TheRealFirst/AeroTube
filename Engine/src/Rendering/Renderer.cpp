#include "atpch.h"

#include "Renderer.h"
#include "glad\glad.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include <filesystem>

/*

Renderer::Renderer(uint32_t width, uint32_t height) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height), model("Assets/Models/bunny/scene.gltf")
{
	// Use the member variables for light properties
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, m_LightPosition);

	glEnable(GL_DEPTH_TEST);

	// When face culling is enabled, use these settings:
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}*/


void Renderer::SetupRenderer()
{
	glEnable(GL_DEPTH_TEST);

	// When face culling is enabled, use these settings:
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void Renderer::Clear(const glm::vec4& color)
{
	// Specify the color of the background
	glClearColor(color.r, color.g, color.b, color.a);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(uint32_t size)
{
	/*
	// Clear any existing errors at start of frame
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		LOG_DEBUG("Clearing previous OpenGL error: 0x%x", err);
	}
	// Verify shader activation
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	if (currentProgram != shaderProgram.ID) {
		LOG_ERROR("Shader activation failed. Expected: %d, Got: %d", shaderProgram.ID, currentProgram);
		return;
	}

	*/

	// Draw model
	// model.Draw(shaderProgram, camera);

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);
}

void Renderer::SetLightColor(Shader shader, glm::vec3 lightColor)
{
	shader.Activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1.0f);
}

