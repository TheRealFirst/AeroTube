#pragma once

#include <unordered_map>
#include <string>
#include "ShaderClass.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture() = default;
	Texture(const char* image, const char* texType, GLuint slot);

	// Static method to load a texture with caching
	static Texture LoadTexture(const char* image, const char* texType, GLuint slot);

	// Clear the texture cache
	static void ClearTextureCache();

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void UnBind();
	void Delete();

	// Static texture cache
	static std::unordered_map<std::string, Texture> s_TextureCache;
};

