#include "atpch.h"

#include "Model.h"

#include "Utils/YamlHelpers.h"
#include "yaml-cpp/yaml.h"


namespace Engine {
	void Model::Draw(const Camera& camera)
	{
		// LOG_DEBUG("Drawing m_Model with %d meshes", m_Meshes.size());
		
		// Check if we have any meshes to draw
		if (m_Meshes.empty()) {
			LOG_WARN("m_Model has no meshes to draw!");
			return;
		}

		for (auto& material : m_Materials)
		{
			material->Bind();
			camera.MatrixUniform(*material->GetShader().get(), "camMatrix");
			camera.PositionUniform(*material->GetShader().get(), "camPos");
			for (auto& mesh : m_Meshes)
			{
				if (mesh.GetMaterial() == material)
				{
					mesh.Draw();
				}
			}
			material->UnBind();
		}
		
	}

	void Model::LoadModel()
	{
		YAML::Node data = YAML::LoadFile(m_FullPath);
		if (!data["Model"] || !data["Meshes"] || !data["Materials"])
		{
			LOG_ERROR("Invalid or corrupt model file: %s", m_FullPath.c_str());
			return;
		}

		m_Name = data["Model"].as<std::string>();
		m_Meshes.clear();
		m_Materials.clear();

		std::filesystem::path modelDir = std::filesystem::path(m_ParentPath);

		// Load materials
		for (const auto& matEntry : data["Materials"])
		{
			std::string matFilename = matEntry.as<std::string>();
			std::filesystem::path matPath = modelDir / matFilename;

			if (!std::filesystem::exists(matPath))
			{
				LOG_WARN("Material file not found: %s", matPath.string().c_str());
				continue;
			}

			Ref<Material> material = CreateRef<Material>(matPath.string());
			m_Materials.push_back(material);
		}

		LOG_DEBUG("Loaded all the Materials")

		// Load meshes
		for (const auto& meshEntry : data["Meshes"])
		{
			std::string meshFilename = meshEntry.as<std::string>();
			std::filesystem::path meshPath = modelDir / meshFilename;

			if (!std::filesystem::exists(meshPath))
			{
				LOG_WARN("Mesh file not found: %s", meshPath.string().c_str());
				continue;
			}

			
			LOG_DEBUG("Started Loading mesh file")
			std::ifstream in(meshPath);
			std::stringstream buffer;
			buffer << in.rdbuf();
			std::string content = buffer.str();
			LOG_DEBUG("Loaded File into memory")
			auto start = std::chrono::high_resolution_clock::now();
			YAML::Node meshData = YAML::Load(content);
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			LOG_DEBUG("YAML parsing took %s ms", std::to_string(duration).c_str());
			LOG_DEBUG("Ended Loading mesh file")
			
			
			
			
			if (!meshData["Name"] || !meshData["Vertices"] || !meshData["Indices"])
			{
				LOG_WARN("Invalid mesh file: %s", meshPath.string().c_str());
				continue;
			}

			MeshAttributes attributes;
			attributes.Name = meshData["Name"].as<std::string>();

			LOG_DEBUG("Created Attributes")

			for (const auto& v : meshData["Vertices"])
				attributes.Vertices.push_back(v.as<Vertex>());

			LOG_DEBUG("Loaded vertices")

			for (const auto& i : meshData["Indices"])
				attributes.Indices.push_back(i.as<uint32_t>());

			LOG_DEBUG("Loaded indices")

			std::string materialName = meshData["Material"] ? meshData["Material"].as<std::string>() : "None";

			if (materialName != "None")
			{
				auto it = std::find_if(m_Materials.begin(), m_Materials.end(), [&](const Ref<Material>& m) {
					return m->GetName() + ".atmat" == materialName;
				});

				if (it != m_Materials.end())
					attributes.Material = *it;
				else
					LOG_WARN("No matching material found for mesh %s: %s", attributes.Name.c_str(), materialName.c_str());
			}

			LOG_DEBUG("Assigned Material")

			m_Meshes.emplace_back(attributes);
			LOG_DEBUG("Created Mesh")
		}

		LOG_DEBUG("Successfully loaded model: %s", m_Name.c_str());
	}
}
