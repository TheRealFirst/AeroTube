#pragma once

#include "vector"

namespace Engine {
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(std::vector<uint32_t>& indices);

		void Bind();
		void Unbind();
		void Delete();

		inline uint32_t GetID() { return m_ID; }
	private:
		uint32_t m_ID = 0;
	};
}
