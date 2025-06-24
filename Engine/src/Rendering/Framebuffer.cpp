#include "atpch.h"
#include "Framebuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Engine
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		AT_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
