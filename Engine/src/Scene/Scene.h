#pragma once

#include "glm\glm.hpp"
#include "./Rendering/Skybox.h"

class Scene
{
public:
	Scene();

	void ToggleSkybox(bool toggleSkybox);
	void LoadSkyboxTextures();
	void SetClearColor(glm::vec4 clearColor);
	void ToggleGrid(bool toggleGrid);

	glm::vec4 GetClearColor();
	void DrawScene(glm::vec3 camPos, glm::vec3 camOrientation, glm::vec3 camUp, uint32_t width, uint32_t height);

private:
	bool m_ToggleSkybox = false;
	bool m_ToggleGrid = true;
	glm::vec4 m_ClearColor = glm::vec4(0.07f, 0.13f, 0.17f, 1.0f);
	Skybox m_Skybox;
};

