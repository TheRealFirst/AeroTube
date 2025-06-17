#include "atpch.h"
#include "Skybox.h"
#include "stb_image.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer.h"


namespace Engine {
	std::vector<Vertex> skyboxVertices = {
		// position                  // normal            // color            // texUV
		{{-1.0f,  1.0f, -1.0f}, {0, 1, 0}, {1, 0, 0}, {0, 0}}, // 0
		{{ 1.0f,  1.0f, -1.0f}, {0, 1, 0}, {0, 1, 0}, {1, 0}}, // 1
		{{ 1.0f,  1.0f,  1.0f}, {0, 1, 0}, {0, 0, 1}, {1, 1}}, // 2
		{{-1.0f,  1.0f,  1.0f}, {0, 1, 0}, {1, 1, 0}, {0, 1}}, // 3
		{{-1.0f, -1.0f, -1.0f}, {0, -1, 0}, {1, 0, 1}, {0, 0}}, // 4
		{{ 1.0f, -1.0f, -1.0f}, {0, -1, 0}, {0, 1, 1}, {1, 0}}, // 5
		{{ 1.0f, -1.0f,  1.0f}, {0, -1, 0}, {1, 1, 1}, {1, 1}}, // 6
		{{-1.0f, -1.0f,  1.0f}, {0, -1, 0}, {0, 0, 0}, {0, 1}}, // 7
	};

	std::vector<unsigned int> skyboxIndices =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	Skybox::Skybox() : m_SkyboxShader("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag")
	{

		m_SkyboxShader.Activate();
		m_SkyboxShader.SetInt("skybox", 0);

		LOG_DEBUG("Creating Vertex Array in Skybox.cpp");
		m_VertexArray.CreateArrays(skyboxVertices, skyboxIndices);


		glGenTextures(1, &m_CubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void Skybox::LoadSkybox(Skybox_Textures* textures)
	{

		int width, height, nrChannels;
		unsigned char* data = stbi_load(textures->right, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading right Skybox Texture")
				stbi_image_free(data);
		}

		data = stbi_load(textures->left, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading left Skybox Texture")
				stbi_image_free(data);
		}

		data = stbi_load(textures->top, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading top Skybox Texture")
				stbi_image_free(data);
		}

		data = stbi_load(textures->bottom, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading bottom Skybox Texture")
				stbi_image_free(data);
		}

		data = stbi_load(textures->front, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading front Skybox Texture")
				stbi_image_free(data);
		}

		data = stbi_load(textures->back, &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Failed Loading back Skybox Texture")
				stbi_image_free(data);
		}
	}

	void Skybox::DrawSkybox(Camera camera)
	{
		glDepthFunc(GL_LEQUAL);

		m_SkyboxShader.Activate();
		m_SkyboxShader.SetMat4("viewproj", camera.GetViewProjection());

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		m_VertexArray.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		Renderer::Render(36);
		m_VertexArray.Bind();

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);
	}
}
