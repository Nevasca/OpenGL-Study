#include "Mesh.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
    m_VAO = std::make_unique<VertexArray>();

    m_VBO = std::make_unique<VertexBuffer>(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(Vertex)), false);

    VertexBufferLayout layout{};
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);

    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = std::make_unique<IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));

    m_IBO->Unbind();
    m_VAO->Unbind();
}

Mesh::Mesh(const void* verticesData, unsigned int verticesSize, const VertexBufferLayout& layout, const std::vector<unsigned>& indices)
{
    m_VAO = std::make_unique<VertexArray>();

    m_VBO = std::make_unique<VertexBuffer>(verticesData, verticesSize, false);
    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = std::make_unique<IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));

    m_IBO->Unbind();
    m_VAO->Unbind();
}
