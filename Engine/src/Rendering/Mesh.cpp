#include "atpch.h"
#include "Mesh.h"

#include "RenderCommand.h"


namespace Engine {
	Mesh::Mesh(const MeshAttributes& attributes)
	{
		m_Attributes = attributes;

		m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(m_Attributes.Vertices.size() * sizeof(Vertex));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPos"},
			{ShaderDataType::Float3, "aNormal"},
			{ShaderDataType::Float3, "aColor"},
			{ShaderDataType::Float2, "aTex"},
		});
		
		vertexBuffer->SetData(m_Attributes.Vertices.data(), m_Attributes.Vertices.size() * sizeof(Vertex));
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Attributes.Indices.data(), m_Attributes.Indices.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Mesh::Draw(glm::mat4 matrix,
		glm::vec3 translation,
		glm::quat rotation,
		glm::vec3 scale)
	{
		m_VertexArray->Bind();
		m_Attributes.Material->GetShader()->SetMat4("model", matrix);
		RenderCommand::DrawIndexed(m_VertexArray, m_Attributes.Indices.size());
		m_VertexArray->UnBind();
	}

	void Mesh::Delete()
	{
	}
}