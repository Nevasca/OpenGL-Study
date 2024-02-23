#pragma once
#include <memory>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Texture;
class Shader;

namespace Breakout
{
    class SpriteRenderer
    {
    public:
        SpriteRenderer(const std::shared_ptr<Shader>& inShader);
        ~SpriteRenderer();

        void Draw(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2{10.f, 10.f}, float rotate = 0.f, glm::vec3 color = glm::vec3{1.f});

    private:

        std::shared_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
    };
}
