#include "atpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Engine
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		AT_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
