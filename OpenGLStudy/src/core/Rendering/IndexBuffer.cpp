#include "IndexBuffer.h"

#include "Core.h"

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
    if(!bIsBound)
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        bIsBound = true;
    }
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    bIsBound = false;
}
