#pragma once
#include "Core.h"
#include "../Rendering/window.h"


int main(int argc, char** argv);

namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		Window* m_Window;

		static Application& Get()
		{
			return *s_Instance;
		}
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}


