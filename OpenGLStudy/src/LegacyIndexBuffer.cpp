#include "LegacyIndexBuffer.h"

#include "Core.h"

LegacyIndexBuffer::LegacyIndexBuffer(const unsigned int* Data, unsigned int Count)
    : m_Count(Count)
{
    // We are using sizeof(unsigned int) when setting the index buffer data instead of using OpenGL uint typedef
    // This is just a precaution in case we are on a platform that has a different byte size for int
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Select the buffer to use with how it`s going to be used
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(unsigned int), Data, GL_STATIC_DRAW)); // Static means it will be modified once and used many times, and to draw
}

LegacyIndexBuffer::~LegacyIndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID)); // Deletes the buffer
}

void LegacyIndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Select the buffer to use with how it`s going to be used
}

void LegacyIndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Unbind by binding it to 0
}
