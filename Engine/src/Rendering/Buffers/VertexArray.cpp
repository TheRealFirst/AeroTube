#include "atpch.h"
#include "VertexArray.h"
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Rendering/Renderer.h"

namespace Engine {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		AT_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
