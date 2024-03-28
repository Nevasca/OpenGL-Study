#pragma once
#include <memory>
#include "VertexBuffer.h"

class VertexArray;

class InstancedArray
{
public:

    InstancedArray(const void* data, unsigned int size, bool bIsDynamic);

    void SetupInstancedAttributesFor(const VertexArray& vertexArray) const;
    void SetSubData(const void* data, unsigned int size) const;
    
    void Bind() const;
    void Unbind() const;
    
private:

    std::unique_ptr<VertexBuffer> m_VBO{};
};
