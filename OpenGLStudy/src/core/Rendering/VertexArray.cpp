#include "VertexArray.h"

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

void VertexArray::AddBuffer(const VertexBuffer& Buffer, const VertexBufferLayout& Layout)
{
    Bind();
    Buffer.Bind();

    const auto& Elements = Layout.GetElements();
    unsigned int Offset = 0;

    for(unsigned int i = 0; i < Elements.size(); i++)
    {
        const auto& Element = Elements[i];
        
        GLCall(glEnableVertexAttribArray(i)); 
        GLCall(glVertexAttribPointer(i, Element.Count, Element.Type, Element.IsNormalized, Layout.GetStride(), (const void*)Offset));

        Offset += Element.Count * VertexBufferElement::GetSizeOfType(Element.Type);
    }
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
