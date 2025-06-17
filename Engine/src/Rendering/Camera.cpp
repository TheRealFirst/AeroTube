#include "atpch.h"
#include "Camera.h"

#include <GLFW\glfw3.h>

#include "Core\Input.h"

#include "glm\glm.hpp"

#include "../Core/Application.h"

namespace Engine {
	Camera::Camera(uint32_t width, uint32_t height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
	{
		m_Width = width;
		m_Height = height;
		m_Position = position;
		m_FOV = FOVdeg;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		UpdateView();
	}

	void Camera::UpdateView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
		m_ViewProjection = m_Projection * m_View;
	}

	void Camera::UpdateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), (float)m_Width / m_Height, m_NearPlane, m_FarPlane);
	}

	void Camera::OnUpdate(Timestep ts)
	{
		Inputs(ts);
		UpdateView();
	}

	void Camera::MatrixUniform(Shader& shader, const char* uniform) const
	{
		shader.SetMat4(uniform, m_ViewProjection);
	}

	void Camera::PositionUniform(Shader& shader, const char* uniform) const
	{
		shader.SetFloat3(uniform, m_Position);
	}

	void Camera::Inputs(Timestep ts)
	{
		// Skip input processing if disabled
		if (!m_ProcessInputs)
			return;

		float speed = m_Speed * ts;
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // TODO: Abstract this!

		// Handles mouse inputs
		if (Input::IsMouseButtonPressed(Mouse::Button0))
		{
			// Hides mouse cursor and enables raw motion for better camera control
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Prevents camera from jumping on the first click
			if (firstClick)
			{
				glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
				firstClick = false;
			}



			// Handles key inputs
			if (Input::IsKeyPressed(Key::W))
			{
				m_Position += speed * m_Orientation;
			}
			if (Input::IsKeyPressed(Key::A))
			{
				m_Position += speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
			}
			if (Input::IsKeyPressed(Key::S))
			{
				m_Position += speed * -m_Orientation;
			}
			if (Input::IsKeyPressed(Key::D))
			{
				m_Position += speed * glm::normalize(glm::cross(m_Orientation, m_Up));
			}
			if (Input::IsKeyPressed(Key::Space))
			{
				m_Position += speed * m_Up;
			}
			if (Input::IsKeyPressed(Key::LeftControl))
			{
				m_Position += speed * -m_Up;
			}

			// Stores the coordinates of the cursor
			glm::vec2 MousePosition = Input::GetMousePosition();


			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees
			float rotX = m_Sensitivity * (float)(MousePosition.y - (m_Height / 2)) / m_Height;
			float rotY = m_Sensitivity * (float)(MousePosition.x - (m_Width / 2)) / m_Width;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				m_Orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);


			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));

		}
		else if (!Input::IsMouseButtonPressed(Mouse::Button0))
		{
			// Restore normal cursor behavior when not controlling camera
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}
	}
}