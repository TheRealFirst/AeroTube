#pragma once

#include "ShaderClass.h"
#include "Buffers\VAO.h"
#include "Buffers\VBO.h"
#include "Buffers\EBO.h"
#include "Texture.h"
#include "glm\glm.hpp"

#include "Camera.h"
#include "Model.h"
#include "Scene\Scene.h"
#include "ImGui/ImGuiLayer.h"


class Renderer
{
public:
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void Clear(const glm::vec4& color);

	void Render(float deltaTime, const Camera& camera);

	// Load a different model
	void LoadModel(const std::string& modelPath);

	void OnWindowResize(uint32_t newWidth, uint32_t newHeight);

	// Model transformation getters and setters
	glm::vec3 GetModelPosition() const { return m_ModelPosition; }
	glm::vec3 GetModelRotation() const { return m_ModelRotation; }
	glm::vec3 GetModelScale() const { return m_ModelScale; }

	void SetModelPosition(const glm::vec3& position) { m_ModelPosition = position; }
	void SetModelRotation(const glm::vec3& rotation) { m_ModelRotation = rotation; }
	void SetModelScale(const glm::vec3& scale) { m_ModelScale = scale; }

	// Light controls
	glm::vec3 GetLightPosition() const { return m_LightPosition; }
	glm::vec4 GetLightColor() const { return m_LightColor; }

	void SetLightPosition(const glm::vec3& position);
	void SetLightColor(const glm::vec4& color);
private:
	uint32_t m_Width;
	uint32_t m_Height;

	Shader shaderProgram;
	Model* model;

	// Model transformation properties
	glm::vec3 m_ModelPosition = glm::vec3(0.0f);
	glm::vec3 m_ModelRotation = glm::vec3(0.0f);
	glm::vec3 m_ModelScale = glm::vec3(1.0f);

	// Light properties
	glm::vec3 m_LightPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec4 m_LightColor = glm::vec4(1.2f, 1.2f, 1.2f, 1.0f);

	Scene* m_TestScene; // TODO: Move to Editorlayer
};

