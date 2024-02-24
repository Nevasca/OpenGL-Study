#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID)); // Deletes the vertex array
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
        
        // Enables the vertex attribute of index i, could be called after the attribute call as well, as long as it's called after bind
        // If we don't enable it, the attribute won't work
        GLCall(glEnableVertexAttribArray(i)); 

        // Define the layout of the buffer
        // index: index of the attribute we are specifying
        // size: count of the types it has
        // type: what type of data it is
        // normalized: should we normalize it to 0-1?
        // stride: bytes offset to next vertex
        // pointer: bytes offset on a single vertex where this attribute starts on (if not zero, we would need to use (const void*)8 for instance)
        GLCall(glVertexAttribPointer(i, Element.Count, Element.Type, Element.IsNormalized, Layout.GetStride(), (const void*)Offset));

        Offset += Element.Count * VertexBufferElement::GetSizeOfType(Element.Type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
