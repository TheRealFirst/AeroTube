#include "atpch.h"
#include "Camera.h"

#include <GLFW\glfw3.h>

#include "Core\Input.h"

#include "glm\glm.hpp"

#include "../Core/Application.h"

Camera::Camera(uint32_t width, uint32_t height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
{
	m_Width = width;
	m_Height = height;
	Position = position;
	m_FOV = FOVdeg;
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
	UpdateView();
}

void Camera::UpdateView()
{
	m_View = glm::lookAt(Position, Position + Orientation, Up);
	m_ViewProjection = m_Projection * m_View;
}

void Camera::UpdateProjection()
{
	m_Projection = glm::perspective(glm::radians(m_FOV), (float)m_Width / m_Height, m_NearPlane, m_FarPlane);
}

void Camera::OnUpdate(Engine::Timestep ts)
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
	shader.SetFloat3(uniform, Position);
}

void Camera::Inputs(Engine::Timestep ts)
{
	// Skip input processing if disabled
	if (!m_ProcessInputs)
		return;

	float speed = m_Speed * ts;
	GLFWwindow* window = static_cast<GLFWwindow*>(Engine::Application::Get().GetWindow().GetWindow()); // TODO: Abstract this!

	// Handles mouse inputs
	if (Engine::Input::IsMouseButtonPressed(Engine::Mouse::Button0))
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
		if (Engine::Input::IsKeyPressed(Engine::Key::W))
		{
			Position += speed * Orientation;
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::A))
		{
			Position += speed * -glm::normalize(glm::cross(Orientation, Up));
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::S))
		{
			Position += speed * -Orientation;
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::D))
		{
			Position += speed * glm::normalize(glm::cross(Orientation, Up));
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::Space))
		{
			Position += speed * Up;
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::LeftControl))
		{
			Position += speed * -Up;
		}

		// Stores the coordinates of the cursor
		glm::vec2 MousePosition = Engine::Input::GetMousePosition();


		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees
		float rotX = m_Sensitivity * (float)(MousePosition.y - (m_Height / 2)) / m_Height;
		float rotY = m_Sensitivity * (float)(MousePosition.x - (m_Width / 2)) / m_Width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);


		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));

	}
	else if (!Engine::Input::IsMouseButtonPressed(Engine::Mouse::Button0))
	{
		// Restore normal cursor behavior when not controlling camera
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
