#include "atpch.h"
#include "Mesh.h"

#include "Renderer.h"


namespace Engine {
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::unordered_map<TextureType2D, Ref<Texture2D>>& textures)
	{
		LOG_DEBUG("Creating mesh with %d vertices, %d indices, %d textures",
			vertices.size(), indices.size(), textures.size());

		Mesh::m_Vertices = vertices;
		Mesh::m_Indices = indices;
		Mesh::m_Textures = textures;

		m_VertexArray.CreateArrays(Mesh::m_Vertices, Mesh::m_Indices);
	}

	void Mesh::Draw(Shader& shader,
		const Camera& camera,
		glm::mat4 matrix,
		glm::vec3 translation,
		glm::quat rotation,
		glm::vec3 scale)
	{

		if (!shader.GetID()) {
			LOG_ERROR("Invalid shader program");
			return;
		}

		shader.Activate();
		m_VertexArray.Bind();

		int slot = 0;
		for (const auto& [type, tex] : m_Textures) {
			std::string uniformName;
			switch (type) {
			case TextureType2D::Diffuse: uniformName = "u_DiffuseMap"; break;
			case TextureType2D::Normal: uniformName = "u_NormalMap"; break;
			case TextureType2D::MetallicRoughness: uniformName = "u_MetallicRoughnessMap"; break;
			case TextureType2D::Emissive: uniformName = "u_EmissiveMap"; break;
			case TextureType2D::Occlusion: uniformName = "u_OcclusionMap"; break;
			default: continue;
			}

			shader.SetInt(uniformName, slot);
			tex->Bind();
			++slot;
		}

		// Take care of the camera Matrix
		camera.PositionUniform(shader, "camPos");
		camera.MatrixUniform(shader, "camMatrix");

		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		glm::mat4 modelMatrix = matrix * glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
		shader.SetMat4("model", modelMatrix);

		Renderer::Render(m_Indices.size());

		m_VertexArray.Unbind();
	}

	void Mesh::Delete()
	{
		m_VertexArray.Delete();
	}
}