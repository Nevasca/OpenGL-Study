#include "VertexBuffer.h"

#include "Core.h"


VertexBuffer::VertexBuffer(const void* Data, unsigned int Size, bool bIsDynamic)
{
    GLCall(glGenBuffers(1, &m_RendererID)); // How many buffers to generate (1) and pointer to set the generated buffer id (&buffer)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Select the buffer to use with how it`s going to be used

    // Static means it will be modified once and used many times, and to draw (like read only)
    // Dynamic means we will dynamically populate the vertex buffer (used for dynamic batch rendering) 
    GLCall(glBufferData(GL_ARRAY_BUFFER, Size, Data, bIsDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID)); // Deletes the buffer
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Select the buffer to use with how it`s going to be used
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Unbind by binding it to 0
}
