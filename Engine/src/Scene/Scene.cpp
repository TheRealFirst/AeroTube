#include "atpch.h"
#include "Scene.h"

#include "Rendering/RenderCommand.h"
#include "Rendering\Renderer3D.h"

namespace Engine {
	Scene::Scene()
	{
		RenderCommand::Init(); // TODO: Move out of here

		// m_Models.emplace_back("Assets/Exported Models/DamagedHelmet/DamagedHelmet.atmod");
		// m_Models.emplace_back("Assets/Exported Models/Bunny/Bunny.atmod");
		m_Models.emplace_back("Assets/Exported Models/Sponza/Sponza.atmod");

		LoadSkyboxTextures();
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

	void Scene::ToggleGrid(bool toggleGrid)
	{
		m_ToggleGrid = toggleGrid;
	}

	void Scene::DrawScene(const Camera& camera)
	{
		RenderCommand::Clear();
		RenderCommand::SetClearColor(m_ClearColor);

		for (uint32_t i = 0; i < m_Models.size(); i++)
		{
			m_Models[i].Draw(camera);
		}

		if (m_DrawSkybox)
			m_Skybox.DrawSkybox(camera);
	}
}