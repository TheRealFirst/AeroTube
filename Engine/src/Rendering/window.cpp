#include "atpch.h"
#include "window.h"
#include <GLFW\glfw3.h>
#include <glad\glad.h>

GLFWwindow* m_Window;

Window::Window(uint32_t width, uint32_t height) {
	m_Width = width;
	m_Height = height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "AeroTube", NULL, NULL);
	if (m_Window == NULL) {
		LOG_FATAL("Failed to create GLFW window.");
		glfwTerminate();
		ASSERT(false);
	}
	glfwMakeContextCurrent(m_Window);

	gladLoadGL();

	glViewport(0, 0, m_Width, m_Height);

	m_Renderer = new Renderer();

	aspectRatio = m_Width / m_Height;
}

Window::~Window() {
	delete m_Renderer;
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
void Window::Run() {
	m_Renderer->Render();

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
bool Window::isRunning() {
	return !glfwWindowShouldClose(m_Window);
}