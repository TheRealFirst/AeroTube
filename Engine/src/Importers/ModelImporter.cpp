#include "atpch.h"
#include "ModelImporter.h"

#include <filesystem>

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "yaml-cpp/yaml.h"

#include "Utils/YamlHelpers.h"

namespace Engine
{
	struct MaterialKey {
		std::unordered_map<TextureType2D, std::string> texturePaths;
		bool operator==(const MaterialKey& other) const {
			return texturePaths == other.texturePaths;
		}
	};

	struct MaterialKeyHash {
		std::size_t operator()(const MaterialKey& k) const {
			std::size_t hash = 0;
			for (const auto& [type, path] : k.texturePaths) {
				hash ^= std::hash<std::string>()(path) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};
	
    void ModelImporter::LoadModel(const std::string& filepath)
    {
        std::filesystem::path modelPath(filepath);
        m_Path = modelPath.parent_path().string();
        m_Name = modelPath.stem().string();
    	
        tinygltf::TinyGLTF loader;
        m_Model = std::make_unique<tinygltf::Model>();
        std::string err;
        std::string warn;

        bool res = loader.LoadASCIIFromFile(m_Model.get(), &err, &warn, filepath);
        if (!warn.empty()) LOG_WARN("GLTF Warning: %s", warn.c_str());
        if (!err.empty()) LOG_ERROR("GLTF Error: %s", err.c_str());

        if (!res) {
            LOG_ERROR("Failed to load glTF: %s", filepath.c_str());
            return;
        }
        LOG_INFO("Loaded glTF: %s", filepath.c_str());

        m_TextureCache.clear();
		std::unordered_map<MaterialKey, Ref<Material>, MaterialKeyHash> materialCache;

		for (const auto& material : m_Model->materials)
		{
			MaterialProbs probs;
			probs.Shader = Shader::Create("Assets/Shaders/default.glsl");

			MaterialKey key;
			auto loadTexture = [&](int index, TextureType2D type, Ref<Texture2D>& target) {
				if (index < 0 || index >= m_Model->textures.size()) return;
				const auto& tex = m_Model->textures[index];
				const auto& image = m_Model->images[tex.source];
				std::string fullPath = m_Path + "/" + image.uri;
				key.texturePaths[type] = fullPath;

				if (!std::filesystem::exists(fullPath)) {
					LOG_WARN("Texture file not found: %s", fullPath.c_str());
				}

				if (!m_TextureCache.count(fullPath))
				{
					m_TextureCache[fullPath] = Texture2D::Create(fullPath, type);
				}
				
				target = m_TextureCache[fullPath];
			};

			loadTexture(material.pbrMetallicRoughness.baseColorTexture.index, TextureType2D::Diffuse, probs.AlbedoTexture);
			loadTexture(material.normalTexture.index, TextureType2D::Normal, probs.NormalTexture);
			loadTexture(material.pbrMetallicRoughness.metallicRoughnessTexture.index, TextureType2D::MetallicRoughness, probs.MetallicRoughnessTexture);
			loadTexture(material.occlusionTexture.index, TextureType2D::Occlusion, probs.OcclusionTexture);
			loadTexture(material.emissiveTexture.index, TextureType2D::Emissive, probs.EmissiveTexture);

			probs.Albedo = glm::make_vec4(material.pbrMetallicRoughness.baseColorFactor.data());
			probs.Metallic = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
			probs.Roughness = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
			probs.Occlusion = material.occlusionTexture.strength;
			probs.Emission = material.emissiveFactor.empty() ? 0.0f : glm::length(glm::make_vec3(material.emissiveFactor.data()));

			if (!materialCache.count(key)) {
				materialCache[key] = CreateRef<Material>(probs, m_Name + std::to_string(materialCache.size()));
			}

			m_Materials.push_back(materialCache[key]);
		}
		
		int sceneIndex = m_Model->defaultScene > -1 ? m_Model->defaultScene : 0;
		const auto& scene = m_Model->scenes[sceneIndex];
		for (int nodeIndex : scene.nodes) {
			ProcessNode(nodeIndex, glm::mat4(1.0f));  // Identity for root
		}

    	LOG_DEBUG("Meshes: %d, Materials %d", m_Meshes.size(), m_Materials.size())
    }

	void ModelImporter::SaveModel(const std::string& filepath, const std::optional<std::string>& name)
    {
    	if (!name->empty())
    		m_Name = name.value();

    	LOG_DEBUG(m_Name.c_str());

    	std::filesystem::path workingPath = filepath;
    	workingPath /= m_Name;
    	std::string workingDir = workingPath.string();
    	LOG_INFO(workingDir.c_str());

    	if (!std::filesystem::create_directory(workingPath))
    	{
    		LOG_ERROR("Couldn't create the folder %s", workingPath.string().c_str());
    		return;
    	}

    	std::string newModelPath = workingDir + "/" + m_Name + ".atmod";

    	YAML::Emitter out;
    	out << YAML::BeginMap;
    	out << YAML::Key << "Model" << YAML::Value << m_Name;
    	out << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq;

    	for (const auto& mesh : m_Meshes)
    	{
    		std::string meshFilename = mesh.name + ".atmesh";
    		out << meshFilename;

    		YAML::Emitter meshOut;
    		meshOut << YAML::BeginMap;
    		meshOut << YAML::Key << "Name"         << YAML::Value << mesh.name;
    		meshOut << YAML::Key << "VertexCount"  << YAML::Value << static_cast<uint32_t>(mesh.vertices.size());
    		meshOut << YAML::Key << "IndexCount"   << YAML::Value << static_cast<uint32_t>(mesh.indices.size());
    		meshOut << YAML::Key << "Material"     << YAML::Value 
					<< (mesh.material ? mesh.material->GetName() + ".atmat" : "None");

    		meshOut << YAML::Key << "Vertices" << YAML::Value << YAML::BeginSeq;
    		for (const auto& v : mesh.vertices)
    			meshOut << v; // uses operator<< for Vertex
    		meshOut << YAML::EndSeq;

    		meshOut << YAML::Key << "Indices" << YAML::Value << YAML::BeginSeq;
    		for (auto index : mesh.indices)
    			meshOut << index;
    		meshOut << YAML::EndSeq;

    		meshOut << YAML::EndMap;
    		
    		std::ofstream meshFile(workingDir + "/" + meshFilename);
    		if (!meshFile.is_open())
    		{
    			LOG_ERROR("Failed to write mesh file: %s", meshFilename.c_str());
    			continue;
    		}
    		meshFile << meshOut.c_str();
    		meshFile.close();
    	}

    	out << YAML::EndSeq;

    	
    	out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;

	    for (const auto& material : m_Materials)
	    {
		    material->SaveMaterial(workingDir);
	    	out << material->GetName() + ".atmat";
	    }

    	out << YAML::EndSeq;
    	out << YAML::EndMap;

    	std::ofstream fout(newModelPath);
    	if (!fout.is_open())
    	{
    		LOG_ERROR("Failed to open file for writing: %s", newModelPath.c_str());
    		return;
    	}
    	fout << out.c_str();

    	LOG_DEBUG("Finished exporting the Model");
    }

    void ModelImporter::ProcessNode(int nodeIndex, const glm::mat4& parentTransform)
    {
    	const tinygltf::Node& node = m_Model->nodes[nodeIndex];

    	glm::mat4 localTransform = glm::mat4(1.0f);

    	if (node.matrix.size() == 16) {
    		localTransform = glm::make_mat4(node.matrix.data());
    	}
    	else {
    		if (node.translation.size() == 3)
    			localTransform = glm::translate(localTransform, glm::vec3(
					node.translation[0], node.translation[1], node.translation[2]));
    		if (node.rotation.size() == 4)
    			localTransform *= glm::mat4_cast(glm::quat(
					node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2])); // WXYZ
    		if (node.scale.size() == 3)
    			localTransform = glm::scale(localTransform, glm::vec3(
					node.scale[0], node.scale[1], node.scale[2]));
    	}

    	glm::mat4 globalTransform = parentTransform * localTransform;

    	// LOG_DEBUG((char*)m_Model->meshes.size());

    	uint32_t index = 0;
    	if (node.mesh >= 0) {
    		const tinygltf::Mesh& mesh = m_Model->meshes[node.mesh];
    		for (const auto& primitive : mesh.primitives) {
    			if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
    				ProcessPrimitive(primitive, globalTransform, primitive.material, index);
    				index++;
    			}
    		}
    	}

    	for (int childIndex : node.children) {
    		ProcessNode(childIndex, globalTransform);
    	}
    }

    void ModelImporter::ProcessPrimitive(const tinygltf::Primitive& primitive, const glm::mat4& transform,
	    int materialIndex, uint32_t index)
    {
    	LOG_DEBUG("Process Primitive")
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		const auto& posAccessor = m_Model->accessors.at(primitive.attributes.at("POSITION"));
		const auto& posBufferView = m_Model->bufferViews.at(posAccessor.bufferView);
		const auto& posBuffer = m_Model->buffers.at(posBufferView.buffer);
		const float* posData = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);

		const float* normalData = nullptr;
		if (primitive.attributes.count("NORMAL")) {
			const auto& normAccessor = m_Model->accessors.at(primitive.attributes.at("NORMAL"));
			const auto& normBufferView = m_Model->bufferViews.at(normAccessor.bufferView);
			const auto& normBuffer = m_Model->buffers.at(normBufferView.buffer);
			normalData = reinterpret_cast<const float*>(&normBuffer.data[normBufferView.byteOffset + normAccessor.byteOffset]);
		}

		const float* uvData = nullptr;
		size_t uvStride = 0;
		if (primitive.attributes.count("TEXCOORD_0")) {
			const auto& accessor = m_Model->accessors[primitive.attributes.at("TEXCOORD_0")];
			const auto& view = m_Model->bufferViews[accessor.bufferView];
			const auto& buffer = m_Model->buffers[view.buffer];
			uvData = reinterpret_cast<const float*>(&buffer.data[view.byteOffset + accessor.byteOffset]);
			uvStride = view.byteStride ? view.byteStride : 2 * sizeof(float);
		}

		vertices.reserve(posAccessor.count);
		for (size_t i = 0; i < posAccessor.count; ++i) {
			Vertex v;
			v.position = glm::vec3(transform * glm::vec4(ReadVec3(posData + i * 3), 1.0f));
			v.normal = normalData ? glm::normalize(glm::mat3(glm::transpose(glm::inverse(transform))) * ReadVec3(normalData + i * 3)) : glm::vec3(0.0f);
			v.color = glm::vec3(1.0f);
			if (uvData)
				v.texUV = glm::vec2(*reinterpret_cast<const float*>((const uint8_t*)uvData + i * uvStride), *reinterpret_cast<const float*>((const uint8_t*)uvData + i * uvStride + sizeof(float)));
			else
				v.texUV = glm::vec2(0.0f);

			v.texUV = {v.texUV.x, 1 - v.texUV.y};
			vertices.push_back(v);
		}

		const auto& idxAccessor = m_Model->accessors.at(primitive.indices);
		const auto& idxBufferView = m_Model->bufferViews.at(idxAccessor.bufferView);
		const auto& idxBuffer = m_Model->buffers.at(idxBufferView.buffer);
		const unsigned char* idxData = &idxBuffer.data[idxBufferView.byteOffset + idxAccessor.byteOffset];

		indices.reserve(idxAccessor.count);
		for (size_t i = 0; i < idxAccessor.count; ++i) {
			if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
				indices.push_back(reinterpret_cast<const uint16_t*>(idxData)[i]);
			else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
				indices.push_back(reinterpret_cast<const uint32_t*>(idxData)[i]);
			else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
				indices.push_back(reinterpret_cast<const uint8_t*>(idxData)[i]);
		}

		LOG_DEBUG("Primitive material index: %d (m_Materials size: %d)", materialIndex, m_Materials.size());
		Ref<Material> mat = materialIndex >= 0 && materialIndex < m_Materials.size() ? m_Materials[materialIndex] : nullptr;
    	
		m_Meshes.emplace_back(ImporterMesh{vertices, indices, m_Name + std::to_string(index), mat});
    }

    glm::vec3 ModelImporter::ReadVec3(const float* data)
    {
    	return glm::vec3(data[0], data[1], data[2]);
    }

    glm::vec2 ModelImporter::ReadVec2(const float* data)
    {
    	return glm::vec2(data[0], data[1]);
    }
}
