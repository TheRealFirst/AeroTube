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

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);
private:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	uint32_t m_Width, m_Height;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	float speed = 0.1f;
	float sensitivity = 100.0f;

};

