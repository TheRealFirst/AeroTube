#pragma once

namespace Engine {
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
        
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
