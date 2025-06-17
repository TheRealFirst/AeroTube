#include "atpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform\OpenGL\OpenGLTexture.h"

namespace Engine {
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case Renderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }

        AT_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case Renderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }

        AT_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, TextureType2D type)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case Renderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(path, type);
        }

        AT_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}