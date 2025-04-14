#include "atpch.h"
#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>

// Initialize static texture cache
std::unordered_map<std::string, Texture> Texture::s_TextureCache;

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Use mipmaps for better performance at different distances
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Use compressed texture formats for better performance
	if (numColCh == 4)
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_COMPRESSED_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if(numColCh == 3)
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_COMPRESSED_RGB,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_COMPRESSED_RED,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		AT_ASSERT("Automatic Texture type recognition failed.")


	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint tex0uni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(tex0uni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}

Texture Texture::LoadTexture(const char* image, const char* texType, GLuint slot)
{
	// Create a key for the texture cache
	std::string key = std::string(image) + "_" + std::string(texType);

	// Check if the texture is already in the cache
	auto it = s_TextureCache.find(key);
	if (it != s_TextureCache.end())
	{
		// Texture found in cache, return it
		LOG_INFO("Texture loaded from cache: {0}", image);
		return it->second;
	}

	// Texture not in cache, load it
	LOG_INFO("Loading texture: {0}", image);
	Texture texture(image, texType, slot);

	// Add to cache
	s_TextureCache[key] = texture;

	return texture;
}

void Texture::ClearTextureCache()
{
	// Delete all textures in the cache
	for (auto& pair : s_TextureCache)
	{
		pair.second.Delete();
	}

	// Clear the cache
	s_TextureCache.clear();
	LOG_INFO("Texture cache cleared");
}
