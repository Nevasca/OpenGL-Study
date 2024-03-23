#pragma once
#include <memory>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
class Renderer;
class Shader;
class Texture;

struct LegacyVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

class LegacyMesh
{
public:

    LegacyMesh(std::vector<LegacyVertex> vertices, std::vector<unsigned int> indices, const std::vector<std::shared_ptr<Texture>>& textures);
    void Draw(Shader& shader);

private:

    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
    std::vector<std::shared_ptr<Texture>> m_Textures;
};
