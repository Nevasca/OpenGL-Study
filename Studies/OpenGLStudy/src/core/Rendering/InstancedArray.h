#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray;

class InstancedArray
{
public:

    InstancedArray(const void* data, unsigned int size, bool bIsDynamic, VertexBufferLayout&& layout);

    void SetupInstancedAttributesFor(VertexArray& vertexArray);
    void SetSubData(const void* data, unsigned int size) const;
    
    void Bind() const;
    void Unbind() const;
    
private:

    std::unique_ptr<VertexBuffer> m_VBO{};
    VertexBufferLayout m_Layout{};
};
