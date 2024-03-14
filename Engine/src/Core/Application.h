#pragma once
#include "Core.h"
#include "../Rendering/window.h"


namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		Window* m_Window;
	};

	// To be defined in client
	Application* CreateApplication();
}


