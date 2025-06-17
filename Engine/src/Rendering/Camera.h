#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm\gtx/vector_angle.hpp>

#include "../Core/Timestep.h"
#include "ShaderClass.h"

struct GLFWwindow{};

namespace Engine {
	class Camera
	{
	public:
		Camera() = default;
		Camera(uint32_t width, uint32_t height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);

		// Set whether the camera should process inputs
		void SetProcessInputs(bool process) { m_ProcessInputs = process; }

		void OnUpdate(Timestep ts);

		void UpdateView();
		void UpdateProjection();
		void MatrixUniform(Shader& shader, const char* uniform) const;
		void PositionUniform(Shader& shader, const char* uniform) const;
		void Inputs(Timestep ts);

		void Resize(uint32_t newWidth, uint32_t newHeight) { m_Width = newWidth; m_Height = newHeight; UpdateProjection(); }

		inline void SetCameraSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }
		inline void SetCameraSpeed(float speed) { m_Speed = speed; }

		inline float GetCameraSensitivity() const { return m_Sensitivity; }
		inline float GetCameraSpeed() const { return m_Speed; }

		inline glm::mat4 GetViewProjection() { return m_ViewProjection; }


		inline glm::vec3 GetPosition() { return m_Position; }
		inline glm::vec3 GetOrientation() { return m_Orientation; }
		inline glm::vec3 GetUp() { return m_Up; }

		inline void SetPosition(glm::vec3 pos) { m_Position = pos; }
		inline void SetOrientation(glm::vec3 orientation) { m_Orientation = orientation; }
		inline void SetUp(glm::vec3 up) { m_Up = up; }
	private:
		glm::vec3 m_Position{ 0.0f, 0.0f, -2.0f };
		glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_ViewProjection = glm::mat4(1.0f);


		uint32_t m_Width, m_Height;

		float m_FOV, m_NearPlane, m_FarPlane;

		// Prevents the camera from jumping around when first clicking left click
		bool firstClick = true;

		// Whether to process inputs
		bool m_ProcessInputs = true;

		float m_Speed = 1.0f;
		float m_Sensitivity = 50.0f;
	};
}
