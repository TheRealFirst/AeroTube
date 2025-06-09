#include "atpch.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE 
#define TINYGLTF_NO_INCLUDE_JSON  
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <json.h>
#include <stb_image.h>
#include "Model.h"
#include <glm\glm.hpp>
#include "glad\glad.h"



void Model::Draw(Shader& shader, const Camera& camera)
{
	LOG_DEBUG("Drawing m_Model with %d meshes", m_Meshes.size());
	
	// Check if we have any meshes to draw
	if (m_Meshes.empty()) {
		LOG_WARN("m_Model has no meshes to draw!");
		return;
	}

	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		
		m_Meshes[i].Draw(shader, camera);
		// meshes[i].Draw(shader, camera, matricesMeshes[i]);
		
		// Check for errors after each mesh
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			LOG_ERROR("OpenGL error after drawing mesh %d: 0x%x", i, err);
		}
	}
}

void Model::loadModel(std::string &path)
{
	tinygltf::TinyGLTF loader;
	m_Model = std::make_unique<tinygltf::Model>();
	std::string err;
	std::string warn;

	bool res = loader.LoadASCIIFromFile(m_Model.get(), &err, &warn, path);
	if (!warn.empty()) LOG_WARN("GLTF Warning: %s", warn.c_str());
	if (!err.empty()) LOG_ERROR("GLTF Error: %s", err.c_str());

	if (!res) {
		LOG_ERROR("Failed to load glTF: %s", path.c_str());
	}
	else
		LOG_INFO("Loaded glTF:%s", path.c_str());

	for (const auto& material : m_Model->materials) {
		std::unordered_map<TextureType, Engine::Ref<Texture>> matTextures;

		auto loadTextureFromIndex = [&](int index, TextureType texType) {
			if (index < 0 || index >= m_Model->textures.size()) return;

			const auto& gltfTex = m_Model->textures[index];
			const auto& image = m_Model->images[gltfTex.source];
			std::string texPath = image.uri;
			std::string fullPath = m_Path + "/" + texPath;

			// Check if already loaded (optional cache)
			auto tex = Engine::CreateRef<Texture>(fullPath.c_str(), texType, loaded_textures.size());
			loaded_textures.push_back(tex);
			matTextures[texType] = tex;
			};

		// Diffuse/BaseColor
		if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
			loadTextureFromIndex(material.pbrMetallicRoughness.baseColorTexture.index, TextureType::Diffuse);

		// Normal Map
		if (material.normalTexture.index >= 0)
			loadTextureFromIndex(material.normalTexture.index, TextureType::Normal);

		// Metallic-Roughness
		if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
			loadTextureFromIndex(material.pbrMetallicRoughness.metallicRoughnessTexture.index, TextureType::MetallicRoughness);

		// Emissive
		if (material.emissiveTexture.index >= 0)
			loadTextureFromIndex(material.emissiveTexture.index, TextureType::Emissive);

		// Occlusion
		if (material.occlusionTexture.index >= 0)
			loadTextureFromIndex(material.occlusionTexture.index, TextureType::Occlusion);

		// Save per-material texture set (you'll associate it with a mesh later)
		material_textures.push_back(matTextures); // add this vector to Model class
	}

	int sceneIndex = m_Model->defaultScene > -1 ? m_Model->defaultScene : 0;
	const auto& scene = m_Model->scenes[sceneIndex];
	for (int nodeIndex : scene.nodes) {
		ProcessNode(nodeIndex, glm::mat4(1.0f));  // Identity for root
	}
}

void Model::ProcessNode(int nodeIndex, const glm::mat4& parentTransform)
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

	if (node.mesh >= 0) {
		const tinygltf::Mesh& mesh = m_Model->meshes[node.mesh];
		for (const auto& primitive : mesh.primitives) {
			if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
				ProcessPrimitive(primitive , globalTransform);
			}
		}
	}

	for (int childIndex : node.children) {
		ProcessNode(childIndex, globalTransform);
	}
}

void Model::ProcessPrimitive(const tinygltf::Primitive& primitive, const glm::mat4& transform)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::unordered_map<TextureType, Engine::Ref<Texture>> meshTextures;

	if (primitive.material >= 0 && primitive.material < material_textures.size()) {
		meshTextures = material_textures[primitive.material];
	}

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
	if (primitive.attributes.count("TEXCOORD_0")) {
		const auto& uvAccessor = m_Model->accessors.at(primitive.attributes.at("TEXCOORD_0"));
		const auto& uvBufferView = m_Model->bufferViews.at(uvAccessor.bufferView);
		const auto& uvBuffer = m_Model->buffers.at(uvBufferView.buffer);
		uvData = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);
	}

	for (size_t i = 0; i < posAccessor.count; ++i) {
		Vertex v;
		v.position = glm::vec3(transform * glm::vec4(ReadVec3(posData + i * 3), 1.0f));
		v.normal = normalData ? glm::normalize(glm::mat3(glm::transpose(glm::inverse(transform))) * ReadVec3(normalData + i * 3)) : glm::vec3(0.0f);
		v.color = glm::vec3(1.0f);
		v.texUV = uvData ? ReadVec2(uvData + i * 2) : glm::vec2(0.0f);
		vertices.push_back(v);
	}

	const auto& idxAccessor = m_Model->accessors.at(primitive.indices);
	const auto& idxBufferView = m_Model->bufferViews.at(idxAccessor.bufferView);
	const auto& idxBuffer = m_Model->buffers.at(idxBufferView.buffer);
	const unsigned char* idxData = &idxBuffer.data[idxBufferView.byteOffset + idxAccessor.byteOffset];

	for (size_t i = 0; i < idxAccessor.count; ++i) {
		if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
			indices.push_back(reinterpret_cast<const uint16_t*>(idxData)[i]);
		}
		else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
			indices.push_back(reinterpret_cast<const uint32_t*>(idxData)[i]);
		}
		else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
			indices.push_back(reinterpret_cast<const uint8_t*>(idxData)[i]);
		}
	}

	m_Meshes.emplace_back(vertices, indices, meshTextures);
}

glm::vec3 Model::ReadVec3(const float* data) {
	return glm::vec3(data[0], data[1], data[2]);
}

glm::vec2 Model::ReadVec2(const float* data) {
	return glm::vec2(data[0], data[1]);
}