#pragma once

#include <glm/glm.hpp>
#include "ShaderClass.h"

#include "Camera.h"

#include "Buffers\VertexArray.h"

namespace Engine {
	struct Skybox_Textures
	{
		char* right;
		char* left;
		char* top;
		char* bottom;
		char* front;
		char* back;
	};

	class Skybox
	{
	public:
		Skybox();
		void LoadSkybox(Skybox_Textures* textures);
		void DrawSkybox(Camera camera);
	private:
		Shader m_SkyboxShader;

		Ref<VertexArray> m_VertexArray;
		unsigned int m_CubemapTexture;
	};
}
