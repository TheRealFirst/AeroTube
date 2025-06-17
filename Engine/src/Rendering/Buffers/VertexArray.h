#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

typedef signed long long int GLsizeiptr;

namespace Engine {
	class VertexArray
	{
	public:
		
		VertexArray();
		~VertexArray() = default;

		void CreateArrays(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

		void Bind();
		void Unbind();

		void Delete();

		inline uint32_t GetID() { return m_ID; }
	private:
		uint32_t m_ID = 0;

		void LinkAttrib(uint32_t layout, uint32_t numComponents, uint32_t type, GLsizeiptr stride, void* offset);
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};
}
