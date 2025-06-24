#pragma once
#include "Rendering/Buffers/VertexBuffer.h"

namespace Engine
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer() override;

        
        void Bind() const override;
        void UnBind() const override;
        
        void SetData(const void* data, uint32_t size) override;
        
        const BufferLayout& GetLayout() const override {return m_Layout;}
        void SetLayout(const BufferLayout& layout) override {m_Layout = layout;}
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };
}
