#include "atpch.h"
#include "Mesh.h"
#include "glad/glad.h"

#include "OpenGLContext.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	LOG_DEBUG("Creating mesh with %d vertices, %d indices, %d textures", 
		vertices.size(), indices.size(), textures.size());
	
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	m_VertexArray.CreateArrays(Mesh::vertices, Mesh::indices);
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

	// bind appropriate textures
	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].GetType();
		if (type == "texture_diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "texture_specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
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

	GLint vao, vbo, ebo;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

	LOG_DEBUG("VAO: %d, VBO: %d, EBO: %d", vao, vbo, ebo);

	glCheckError();
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glCheckError();
	// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	m_VertexArray.Unbind();
}

void Mesh::Delete()
{
	m_VertexArray.Delete();
}
