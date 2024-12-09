#include "Rendering/VertexBuffer.h"

#include "Rendering/OpenGLCore.h"

namespace Glacirer
{
    namespace Rendering
    {
        VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool bIsDynamic)
        {
            GLCall(glGenBuffers(1, &m_RendererID));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

            GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, bIsDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        }

        VertexBuffer::~VertexBuffer()
        {
            GLCall(glDeleteBuffers(1, &m_RendererID));
        }

        void VertexBuffer::Bind() const
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        }

        void VertexBuffer::Unbind() const
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        void VertexBuffer::SetSubData(const void* data, unsigned int size) const
        {
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
        }
    }
}
