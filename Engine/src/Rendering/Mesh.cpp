#include "atpch.h"
#include "Mesh.h"

#include "Renderer.h"


namespace Engine {
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::unordered_map<TextureType2D, Ref<Texture2D>>& textures)
	{
		LOG_DEBUG("Creating mesh with %d vertices, %d indices, %d textures",
			vertices.size(), indices.size(), textures.size());

		m_Vertices = vertices;
		m_Indices = indices;
		m_Textures = textures;

		m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(m_Vertices.size() * sizeof(Vertex));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPos"},
			{ShaderDataType::Float3, "aNormal"},
			{ShaderDataType::Float3, "aColor"},
			{ShaderDataType::Float2, "aTex"},
		});
		
		vertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Mesh::Draw(Shader& shader,
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
		m_VertexArray->Bind();

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

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		shader.SetMat4("model", modelMatrix);

		Renderer::Render(m_Indices.size());

		m_VertexArray->UnBind();
	}

	void Mesh::Delete()
	{
	}
}