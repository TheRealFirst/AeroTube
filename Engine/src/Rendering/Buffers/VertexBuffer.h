#pragma once

#include "glm\glm.hpp"
#include "vector"


namespace Engine {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texUV;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(std::vector <Vertex>& vertices);

		void Bind();
		void Unbind();
		void Delete();

		inline uint32_t GetID() { return m_ID; }
	private:
		uint32_t m_ID = 0;
	};
}
