#include "VertexArray.h"

#include "Core.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

unsigned int VertexArray::m_LastBoundVertexArrayId = 0;

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
    if(m_LastBoundVertexArrayId != m_RendererID)
    {
        GLCall(glBindVertexArray(m_RendererID));
        m_LastBoundVertexArrayId = m_RendererID;
    }
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
    m_LastBoundVertexArrayId = 0;
}
