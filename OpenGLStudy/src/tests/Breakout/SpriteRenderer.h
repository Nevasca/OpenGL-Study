#pragma once
#include <memory>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class LegacyIndexBuffer;
class VertexBuffer;
class LegacyVertexArray;
class Texture;
class LegacyShader;

namespace Breakout
{
    class SpriteRenderer
    {
    public:
        SpriteRenderer(const std::shared_ptr<LegacyShader>& inShader);
        ~SpriteRenderer();

        void Draw(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2{10.f, 10.f}, float rotate = 0.f, glm::vec3 color = glm::vec3{1.f}) const;

    private:

        std::shared_ptr<LegacyShader> m_Shader;
        std::unique_ptr<LegacyVertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<LegacyIndexBuffer> m_IBO;
    };
}
