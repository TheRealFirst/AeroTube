#include "atpch.h"
#include "Skybox.h"
#include "stb_image.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
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

Skybox::Skybox() : skyboxShader("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag")
{

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Create VAO, VBO, and EBO for the skybox
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
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

	skyboxShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "viewproj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection()));

	// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
	// where an object is present (a depth of 1.0f will always fail against any object's depth value)
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Switch back to the normal depth function
	glDepthFunc(GL_LESS);
}

