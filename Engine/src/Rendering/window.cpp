#include "atpch.h"
#include "window.h"
#include <GLFW\glfw3.h>
#include <glad\glad.h>

GLFWwindow* m_Window;



Window::Window(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {

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

	m_Renderer = new Renderer(m_Width, m_Height, m_Window);

	aspectRatio = m_Width / m_Height;

	lastFrameTime = glfwGetTime();
}

Window::~Window() {
	delete m_Renderer;
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
void Window::Run() {
	float delta = glfwGetTime() - lastFrameTime;
	m_Renderer->Render(delta);

	glfwGetFramebufferSize(m_Window, &newFrameWidth, &newFrameHeight);
	if (newFrameWidth != m_Width || newFrameHeight != m_Height) {
		OnResize(newFrameWidth, newFrameHeight);
	}

	

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
	lastFrameTime = glfwGetTime();
}


bool Window::isRunning() {
	return !glfwWindowShouldClose(m_Window);
}

void Window::OnResize(int width, int height)
{
	LOG_DEBUG("Resized the window.");
	m_Width = width;
	m_Height = height;
	aspectRatio = m_Width / m_Height;
	glViewport(0, 0, m_Width, m_Height);

	m_Renderer->Resize(m_Width, m_Height);
}
