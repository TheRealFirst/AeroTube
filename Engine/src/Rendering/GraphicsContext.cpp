#include "atpch.h"
#include "GraphicsContext.h"
#include "Renderer.h"

#include "Platform\OpenGL\OpenGLContext.h"

namespace Engine {
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case Renderer::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
	}
}