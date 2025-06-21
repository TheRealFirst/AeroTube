#pragma once
#include "Rendering/Buffers/IndexBuffer.h"

namespace Engine
{
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer() override;
        
        virtual void Bind() const override;
        virtual void UnBind() const override;
        
        virtual uint32_t GetCount() const override {return m_Count;}
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}
