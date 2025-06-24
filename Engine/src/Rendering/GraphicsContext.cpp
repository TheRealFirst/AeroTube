#include "atpch.h"
#include "GraphicsContext.h"
#include "RendererAPI.h"

#include "Platform\OpenGL\OpenGLContext.h"

namespace Engine {
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
	}
}