#include "atpch.h"
#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Rendering/RendererAPI.h"

namespace Engine {
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: AT_ASSERT(false, "Renderer::API::None: is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		AT_ASSERT(false, "Unknown RendererAPI!");
		return  nullptr;

	}
}
