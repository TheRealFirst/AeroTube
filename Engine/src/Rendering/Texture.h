#pragma once

#include <unordered_map>
#include <string>
#include "ShaderClass.h"


enum class TextureType {
	Diffuse,
	Normal,
	MetallicRoughness,
	Occlusion,
	Emissive,
	Unknown
};

class Texture
{
public:
	

	Texture(const char* image, TextureType texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	inline TextureType GetType() { return m_Type; }
	inline void SetType(TextureType type) { m_Type = type; }

	inline std::string GetPath() { return m_Path; }
	inline void SetPath(std::string path) { m_Path = path; }

	inline uint32_t GetID() { return m_ID; }
	inline void SetID(uint32_t ID) { m_ID = ID; }

	inline uint32_t GetUnit() { return m_Unit; }
	inline void SetUnit(uint32_t unit) { m_Unit = unit; }

private:
	uint32_t m_ID;
	uint32_t m_Unit;
	TextureType m_Type;
	std::string m_Path;
};

