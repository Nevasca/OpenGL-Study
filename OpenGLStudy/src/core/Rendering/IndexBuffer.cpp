#include "IndexBuffer.h"

#include "OpenGLCore.h"

unsigned int IndexBuffer::m_LastBoundIndexBufferId = 0;

IndexBuffer::IndexBuffer(const unsigned int* Data, unsigned int Count)
    : m_Count(Count)
{
    // We are using sizeof(unsigned int) when setting the index buffer data instead of using OpenGL uint typedef
    // This is just a precaution in case we are on a platform that has a different byte size for int
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(unsigned int), Data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    if(m_LastBoundIndexBufferId != m_RendererID)
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        m_LastBoundIndexBufferId = m_RendererID;
    }
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    m_LastBoundIndexBufferId = 0;
}
