#include "atpch.h"
#include "Application.h"

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		initialize_logging();
		LOG_INFO("Started up the Engine.");

		s_Instance = this;

		m_Window = new Window(3840, 2160);
	}

	Application::~Application()
	{
		delete m_Window;
		LOG_INFO("Shutting the Engine down.");
		shutdown_logging();
	}

	void Application::Run()
	{
		// while (true);
		while (m_Window->isRunning()) {
			m_Window->Run();
		}
	}
}

