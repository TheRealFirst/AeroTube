#pragma once

#include "Rendering\Texture.h"

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const std::string& path, TextureType2D type);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetID() const override { return m_ID; }
		virtual TextureType2D GetType() const override { return m_Type; }
		virtual std::string GetTypeAsUniform() const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_ID == ((OpenGLTexture2D&)other).m_ID;
		}

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_ID;
		uint32_t m_InternalFormat, m_DataFormat;
		TextureType2D m_Type;
	};
}