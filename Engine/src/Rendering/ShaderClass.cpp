#include "atpch.h"
#include "ShaderClass.h"
#include "glad\glad.h"

#include <glm\gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Engine {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		AT_ASSERT(false, "Unknown RendererAPI")
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: AT_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case Renderer::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);        }

		AT_ASSERT(false, "Unknown RendererAPI")
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		AT_ASSERT(!Exists(name), "Shader already exists!")
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, Ref<Shader>& shader)
	{
		AT_ASSERT(!Exists(name), "Shader already exists!")
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		AT_ASSERT(Exists(name), "Shader not found!")
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}

