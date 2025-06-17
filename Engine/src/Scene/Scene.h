#pragma once

#include "glm\glm.hpp"
#include "./Rendering/Skybox.h"
#include "Rendering\Camera.h"
#include "Rendering\Model.h"

namespace Engine {
	class Scene
	{
	public:
		Scene();

		void ToggleSkybox(bool toggleSkybox);
		void LoadSkyboxTextures();
		void SetClearColor(glm::vec4 clearColor);
		void ToggleGrid(bool toggleGrid);

		glm::vec4 GetClearColor();
		void DrawScene(Camera camera);

	private:
		bool m_DrawSkybox = false;
		bool m_ToggleGrid = true;
		glm::vec4 m_ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1);
		glm::vec3 m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		Skybox m_Skybox;

		std::vector<Model> m_Models;
		std::vector<Shader> m_Shaders;
	};
}
