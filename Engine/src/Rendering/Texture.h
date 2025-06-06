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
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	inline std::string GetType() { return m_Type; }
	inline void SetType(std::string type) { m_Type = type; }

	inline std::string GetPath() { return m_Path; }
	inline void SetPath(std::string path) { m_Path = path; }

private:
	std::string m_Type;
	std::string m_Path;
};

