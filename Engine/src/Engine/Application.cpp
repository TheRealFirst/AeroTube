#include "atpch.h"
#include "Application.h"

namespace Engine {
	GLFWwindow* m_Window;

	Application::Application()
	{
		initialize_logging();
		LOG_INFO("Started up the Engine.");

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(800, 800, "AeroTube", NULL, NULL);
		if (m_Window == NULL) {
			LOG_FATAL("Failed to create GLFW window.");
			glfwTerminate();
			ASSERT(false);
		}
		glfwMakeContextCurrent(m_Window);

		gladLoadGL();

		glViewport(0, 0, 800, 800);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_Window);
	}

	Application::~Application()
	{
		LOG_INFO("Shutting the Engine down.");
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		shutdown_logging();
	}

	void Application::Run()
	{
		// while (true);
		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
		}
	}
}

