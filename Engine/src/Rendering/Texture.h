#pragma once

#include <unordered_map>
#include <string>
#include "ShaderClass.h"

namespace Engine {

	enum class TextureType2D {
		Diffuse,
		Normal,
		MetallicRoughness,
		Occlusion,
		Emissive,
		Unknown
	};

	class Texture2D;

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetID() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const std::string& path, TextureType2D type);

		virtual TextureType2D GetType() const = 0;
		virtual std::string GetTypeAsUniform() const = 0;
	};
}
