#include "atpch.h"
#include "Application.h"

namespace Engine {
	Application::Application()
	{
		initialize_logging();
		LOG_INFO("Started up the Engine.");

		m_Window = new Window(1600, 1600);
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

