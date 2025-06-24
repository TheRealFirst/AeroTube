#pragma once
#include "Core\Window.h"
#include "Rendering/GraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProbs& probs);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;


		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProbs& probs);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
