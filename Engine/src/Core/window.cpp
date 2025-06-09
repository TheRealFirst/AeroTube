#include "atpch.h"
#include "window.h"
#include <GLFW\glfw3.h>

#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"



namespace Engine {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) 
	{
		LOG_ERROR("GLFW Error");
		LOG_ERROR(description);
	}

	Window::Window(const std::string& title, uint32_t width, uint32_t height)
	{
		m_Data.Title = title;
		m_Data.Width = width;
		m_Data.Height = height;
		
		if (s_GLFWWindowCount == 0) 
		{
			int success = glfwInit();
			AT_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		
        #ifdef AT_DEBUG
        if (Renderer::GetAPI() == Renderer::API::OpenGL)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        #endif
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
		++s_GLFWWindowCount;

        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

		if (m_Window == NULL) {
			LOG_FATAL("Failed to create GLFW window.");
			glfwTerminate();
			AT_ASSERT(false);
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);


        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
	}

	Window::~Window() {
		glfwDestroyWindow(m_Window);

        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0)
        {
		    glfwTerminate();
        }
	}
	void Window::OnUpdate(Timestep ts) {

		glfwPollEvents();
        m_Context->SwapBuffers();
	}


    void Window::SetVSync(bool enabled)
    {
            if (enabled)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }


    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }
}