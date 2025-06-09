#pragma once

#include "ShaderClass.h"
#include "Texture.h"
#include "glm\glm.hpp"

#include "Camera.h"
#include "Model.h"
#include "Scene\Scene.h"
#include "ImGui/ImGuiLayer.h"


class Renderer
{
public:
	enum class API
	{
		None = 0,
		OpenGL = 1
	};
public:
	static void Init();

	static void Clear(const glm::vec4& color);

	static void Render(uint32_t size);

	static API GetAPI() { return s_API; }
private:
	static API s_API;
};

