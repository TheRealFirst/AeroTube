#include "atpch.h"
#include"VertexBuffer.h"
#include <glad\glad.h>

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Rendering/Renderer.h"

namespace Engine {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "Renderer::API::None: is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}
		AT_ASSERT(false, "Unknown RendererAPI!");
		return  nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "Renderer::API::None: is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		AT_ASSERT(false, "Unknown RendererAPI!");
		return  nullptr;
	}
}
