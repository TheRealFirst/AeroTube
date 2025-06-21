#include "atpch.h"
#include"IndexBuffer.h"
#include <glad\glad.h>

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Rendering/Renderer.h"

namespace Engine {
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "Renderer::API::None: is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		AT_ASSERT(false, "Unknown RendererAPI!");
		return  nullptr;

	}
}
