#include "atpch.h"
#include "Scene.h"

Scene::Scene() : m_Skybox()
{
	LoadSkyboxTextures();
}

void Scene::ToggleSkybox(bool toggleSkybox)
{
	m_ToggleSkybox = toggleSkybox;
}

void Scene::LoadSkyboxTextures()
{
	Skybox_Textures SkyboxTextures;
	SkyboxTextures.front = "Assets/Cubemaps/front.jpg";
	SkyboxTextures.back = "Assets/Cubemaps/back.jpg";
	SkyboxTextures.top = "Assets/Cubemaps/top.jpg";
	SkyboxTextures.bottom = "Assets/Cubemaps/bottom.jpg";
	SkyboxTextures.right = "Assets/Cubemaps/right.jpg";
	SkyboxTextures.left = "Assets/Cubemaps/left.jpg";
	m_Skybox.LoadSkybox(&SkyboxTextures);
}

void Scene::SetClearColor(glm::vec4 clearColor)
{
	m_ClearColor = clearColor;
}

void Scene::ToggleGrid(bool toggleGrid)
{
	m_ToggleGrid = toggleGrid;
}

glm::vec4 Scene::GetClearColor()
{
	return m_ClearColor;
}

void Scene::DrawScene(glm::vec3 camPos, glm::vec3 camOrientation, glm::vec3 camUp, uint32_t width, uint32_t height)
{
	if (m_ToggleSkybox)
		m_Skybox.DrawSkybox(camPos, camOrientation, camUp, width, height);
}
