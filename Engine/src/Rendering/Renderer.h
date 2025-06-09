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
	static void SetupRenderer();

	static void Clear(const glm::vec4& color);

	static void Render(uint32_t size);

	static void SetLightColor(Shader shader, glm::vec3 lightColor);
};

