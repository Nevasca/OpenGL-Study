#pragma once
#include <memory>
#include <string>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class VertexBufferLayout;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

class Mesh
{
public:

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Mesh(const void* verticesData, unsigned int verticesSize, const VertexBufferLayout& layout, const std::vector<unsigned int>& indices);

    VertexArray& GetVertexArray() const { return *m_VAO; }
    IndexBuffer& GetIndexBuffer() const { return *m_IBO; }
    void SetName(const std::string& name) { m_Name = name; }
    std::string GetName() const { return m_Name; }

private:

    std::unique_ptr<VertexArray> m_VAO{};
    std::unique_ptr<VertexBuffer> m_VBO{};
    std::unique_ptr<IndexBuffer> m_IBO{};
    std::string m_Name{};
};
