#include "atpch.h"
#include "Scene.h"

#include "Rendering\Renderer.h"

Scene::Scene()
{
	Renderer::SetupRenderer();

	m_Models.emplace_back("Assets/Models/bunny/scene.gltf");
	m_Shaders.emplace_back("Assets/Shaders/default.vert", "Assets/Shaders/default.frag");

	m_Skybox = Skybox();
	LoadSkyboxTextures();

	Renderer::SetLightColor(m_Shaders[0], m_LightColor);
}

void Scene::ToggleSkybox(bool toggleSkybox)
{
	m_DrawSkybox = toggleSkybox;
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

void Scene::DrawScene(Camera camera)
{
	Renderer::Clear(m_ClearColor);

	for (uint32_t i = 0; i < m_Models.size(); i++)
	{
		m_Models[i].Draw(m_Shaders[0], camera);
	}

	if (m_DrawSkybox)
		m_Skybox.DrawSkybox(camera);
}
