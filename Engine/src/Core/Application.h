#pragma once
#include "Core.h"
#include "window.h"
#include "../Rendering/Renderer.h"
#include "Layerstack.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/Event.h"

#include "../ImGui/ImGuiLayer.h"

#include "Timestep.h"


int main(int argc, char** argv);

namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }


		static Application& Get()
		{
			return *s_Instance;
		}


	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


	private:
		ImGuiLayer* m_ImGuiLayer;
		Window* m_Window;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}


