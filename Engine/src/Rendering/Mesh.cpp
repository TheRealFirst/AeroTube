#include "atpch.h"
#include "Mesh.h"
#include "glad/glad.h"

#include "OpenGLContext.h"
#include "Renderer.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::unordered_map<TextureType, Engine::Ref<Texture>> textures)
{
	LOG_DEBUG("Creating mesh with %d vertices, %d indices, %d textures", 
		vertices.size(), indices.size(), textures.size());
	
	Mesh::m_Vertices = vertices;
	Mesh::m_Indices = indices;
	Mesh::m_Textures = textures;

	m_VertexArray.CreateArrays(Mesh::m_Vertices, Mesh::m_Indices);
	glCheckError();
}

void Mesh::Draw(Shader& shader,
	const Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale)
{

	if (!shader.ID) {
		LOG_ERROR("Invalid shader program");
		return;
	}

	shader.Activate();
	m_VertexArray.Bind();

	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	if (currentProgram != shader.ID) {
		LOG_ERROR("Shader program mismatch! Expected: %d, Got: %d", shader.ID, currentProgram);
	}

	int slot = 0;
	for (const auto& [type, tex] : m_Textures) {
		std::string uniformName;
		switch (type) {
		case TextureType::Diffuse: uniformName = "u_DiffuseMap"; break;
		case TextureType::Normal: uniformName = "u_NormalMap"; break;
		case TextureType::MetallicRoughness: uniformName = "u_MetallicRoughnessMap"; break;
		case TextureType::Emissive: uniformName = "u_EmissiveMap"; break;
		case TextureType::Occlusion: uniformName = "u_OcclusionMap"; break;
		default: continue;
		}

		tex->texUnit(shader, uniformName.c_str(), slot);
		tex->Bind();
		++slot;
	}

	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.MatrixUniform(shader, "camMatrix");

	LOG_DEBUG("Using VAO: %d", m_VertexArray.ID);

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glm::mat4 modelMatrix = matrix * glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glCheckError();
	// Draw the actual mesh
	// glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, (void*)0);
	glCheckError();

	Renderer::Render(m_Indices.size());

	// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	m_VertexArray.Unbind();
}

void Mesh::Delete()
{
	m_VertexArray.Delete();
}
