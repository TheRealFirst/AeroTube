#include "atpch.h"
#include "Application.h"
#include "../Logging/Log.h"
#include "../Logging/asserts.h"


namespace Engine {
	Application::Application()
	{
		initialize_logging();
		LOG_INFO("Started up the Engine.")
	}

	Application::~Application()
	{
		LOG_INFO("Shutting the Engine down.")
		shutdown_logging();
	}

	void Application::Run()
	{
		while (true);
	}
}

