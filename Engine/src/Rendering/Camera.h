#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm\gtx/vector_angle.hpp>

#include "ShaderClass.h"

struct GLFWwindow{};

class Camera
{
public:
	Camera(uint32_t width, uint32_t height, glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window, float deltaTime);

	void Resize(uint32_t newWidth, uint32_t newHeight) { m_Width = newWidth; m_Height = newHeight; }


	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
private:
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	uint32_t m_Width, m_Height;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	float m_Speed = 2000.0f;
	float sensitivity = 50.0f;
};

