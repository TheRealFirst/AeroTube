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
	// Verify shader is active
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	if (currentProgram != shader.ID) {
		LOG_ERROR("Shader program mismatch! Expected: %d, Got: %d", shader.ID, currentProgram);
		shader.Activate();
	}

	LOG_DEBUG("Drawing m_Model with %d meshes", m_Meshes.size());
	
	// Check if we have any meshes to draw
	if (m_Meshes.empty()) {
		LOG_WARN("m_Model has no meshes to draw!");
		return;
	}

	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		LOG_DEBUG("Drawing mesh %d with %d vertices", i, m_Meshes[i].vertices.size());
		
		// Verify mesh data
		if (m_Meshes[i].vertices.empty() || m_Meshes[i].indices.empty()) {
			LOG_ERROR("Mesh %d has invalid data! Vertices: %d, Indices: %d", 
				i, m_Meshes[i].vertices.size(), m_Meshes[i].indices.size());
			continue;
		}
		
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

	for (const auto& gltfMesh : m_Model->meshes) {
		for (const auto& primitive : gltfMesh.primitives) {
			if (primitive.mode != TINYGLTF_MODE_TRIANGLES) continue;
			ProcessPrimitive(primitive);
		}
	}
}

void Model::ProcessPrimitive(const tinygltf::Primitive& primitive)
{
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
	if (primitive.attributes.count("TEXCOORD_0")) {
		const auto& uvAccessor = m_Model->accessors.at(primitive.attributes.at("TEXCOORD_0"));
		const auto& uvBufferView = m_Model->bufferViews.at(uvAccessor.bufferView);
		const auto& uvBuffer = m_Model->buffers.at(uvBufferView.buffer);
		uvData = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);
	}

	for (size_t i = 0; i < posAccessor.count; ++i) {
		Vertex v;
		v.position = ReadVec3(posData + i * 3);
		v.normal = normalData ? ReadVec3(normalData + i * 3) : glm::vec3(0.0f);
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

	m_Meshes.emplace_back(vertices, indices, loaded_textures);
}

glm::vec3 Model::ReadVec3(const float* data) {
	return glm::vec3(data[0], data[1], data[2]);
}

glm::vec2 Model::ReadVec2(const float* data) {
	return glm::vec2(data[0], data[1]);
}