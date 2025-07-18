#include "atpch.h"

#include "Model.h"

#include "Utils/YamlHelpers.h"
#include "yaml-cpp/yaml.h"

#include "mesh_generated.h"

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
		#include "mesh_generated.h" // Generated FlatBuffer header

// ...

	for (const auto& meshEntry : data["Meshes"])
	{
		std::string meshFilename = meshEntry.as<std::string>();
		std::filesystem::path meshPath = modelDir / meshFilename;

		if (!std::filesystem::exists(meshPath))
		{
			LOG_WARN("Mesh file not found: %s", meshPath.string().c_str());
			continue;
		}

		// Load binary file into memory
		std::ifstream file(meshPath, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to open mesh file: %s", meshPath.string().c_str());
			continue;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (!file.read(buffer.data(), size))
		{
			LOG_ERROR("Failed to read mesh file: %s", meshPath.string().c_str());
			continue;
		}

		// Verify buffer
		flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(buffer.data()), size);
		if (!verifier.VerifyBuffer<FBMesh::Mesh>(nullptr))
		{
			LOG_ERROR("Invalid FlatBuffer mesh: %s", meshPath.string().c_str());
			continue;
		}

		const FBMesh::Mesh* meshFB = FBMesh::GetMesh(buffer.data());

		// Fill MeshAttributes
		MeshAttributes attributes;
		attributes.Name = meshFB->name()->str();

		// Load vertices
		auto fbVerts = meshFB->vertices();
		attributes.Vertices.reserve(fbVerts->size());
		for (const auto& v : *fbVerts)
		{
			Engine::Vertex vertex;
			vertex.position = { v->position().x(), v->position().y(), v->position().z() };
			vertex.normal   = { v->normal().x(),   v->normal().y(),   v->normal().z() };
			vertex.color    = { v->color().x(),    v->color().y(),    v->color().z() };
			vertex.texUV    = { v->texcoord().u(), v->texcoord().v() };
			attributes.Vertices.push_back(vertex);
		}

		// Load indices
		auto fbIndices = meshFB->indices();
		attributes.Indices.assign(fbIndices->begin(), fbIndices->end());

		// Link material
		std::string matName = meshFB->material()->str() + ".atmat";
		auto it = std::find_if(m_Materials.begin(), m_Materials.end(), [&](const Ref<Material>& m) {
			return m->GetName() + ".atmat" == matName;
		});
		if (it != m_Materials.end())
		{
			attributes.Material = *it;
		}
		else
		{
			LOG_WARN("No matching material found for mesh %s: %s", attributes.Name.c_str(), matName.c_str());
		}

		// Construct Mesh and store

		LOG_DEBUG("loaded mesh %s", attributes.Name.c_str())
		m_Meshes.emplace_back(attributes);
	}

		LOG_DEBUG("Successfully loaded model: %s", m_Name.c_str());
	}
}
