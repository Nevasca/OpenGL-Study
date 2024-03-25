#pragma once
#include <memory>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "LegacyIndexBuffer.h"
#include "VertexBuffer.h"
#include "LegacyVertexArray.h"
class Renderer;
class LegacyShader;
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
    void Draw(LegacyShader& shader);

private:

    std::unique_ptr<LegacyVertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<LegacyIndexBuffer> m_IBO;
    std::vector<std::shared_ptr<Texture>> m_Textures;
};
