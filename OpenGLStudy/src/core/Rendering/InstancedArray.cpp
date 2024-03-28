#include "InstancedArray.h"

#include "Core.h"
#include "VertexArray.h"
#include <glm/glm.hpp>

InstancedArray::InstancedArray(const void* data, unsigned size, bool bIsDynamic)
{
    m_VBO = std::make_unique<VertexBuffer>(data, size, bIsDynamic);
}

//TODO: this method is only setting instanced model matrix, refactor it for receiving
//a custom layout on other method so we can have other instanced attributes
void InstancedArray::SetupInstancedAttributesFor(const VertexArray& vertexArray) const
{
    Bind();
    vertexArray.Bind();

    // Set attribute pointers for model matrix (4 times vec4)
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
    GLCall(glEnableVertexAttribArray(4));
    GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))));
    GLCall(glEnableVertexAttribArray(5));
    GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))));
    GLCall(glEnableVertexAttribArray(6));
    GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))));

    //Instead of changing this attribute value on shader every new vertex (divisor 0)
    //we want it to change every new instance (divisor 1)
    GLCall(glVertexAttribDivisor(3, 1));
    GLCall(glVertexAttribDivisor(4, 1));
    GLCall(glVertexAttribDivisor(5, 1));
    GLCall(glVertexAttribDivisor(6, 1));

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
