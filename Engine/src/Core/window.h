#pragma once
#include <stdint.h>
#include "Rendering/Renderer.h"
#include "../Core/Timestep.h"
#include "../Events/Event.h"

#include "../Rendering/GraphicsContext.h"

struct GLFWwindow;


namespace Engine {
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const std::string& title, uint32_t width, uint32_t height);
		~Window();
		void OnUpdate(Timestep ts);

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }

		GLFWwindow* GetWindow() const { return m_Window; }


		void SetVSync(bool enabled) ;
		bool IsVSync() const;
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
