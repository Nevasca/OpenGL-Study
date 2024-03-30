#include "VertexArray.h"

#include "Core.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
    Bind();
    buffer.Bind();

    m_NextAttributeLocation = layout.CreateAttributes();
}

void VertexArray::Bind() const
{
    if(!bIsBound)
    {
        GLCall(glBindVertexArray(m_RendererID));
        bIsBound = true;
    }
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
    bIsBound = false;
}
