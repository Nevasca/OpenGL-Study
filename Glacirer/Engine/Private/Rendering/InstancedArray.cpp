#include "Rendering/InstancedArray.h"

#include "Rendering/VertexArray.h"

InstancedArray::InstancedArray(const void* data, unsigned size, bool bIsDynamic, VertexBufferLayout&& layout)
{
    m_VBO = std::make_unique<VertexBuffer>(data, size, bIsDynamic);
    m_Layout = std::move(layout);
}

void InstancedArray::SetupInstancedAttributesFor(VertexArray& vertexArray)
{
    // Prevent setting vertex attributes for instanced again, eventually crashing by passing max attributes per vertex (16)
    if(vertexArray.IsInstancedRenderingConfigured())
    {
        return;
    }

    Bind();
    vertexArray.Bind();

    const unsigned int nextAttributeLocation = m_Layout.CreateAttributes(vertexArray.GetNextAttributeLocation());
    vertexArray.SetNextAttributeLocation(nextAttributeLocation);
    vertexArray.SetIsInstancedRenderingConfigured(true);

    vertexArray.Unbind();
    Unbind();
}

void InstancedArray::SetSubData(const void* data, unsigned size) const
{
    m_VBO->SetSubData(data, size);
}

void InstancedArray::Bind() const
{
    m_VBO->Bind();
}

void InstancedArray::Unbind() const
{
    m_VBO->Unbind();
}
