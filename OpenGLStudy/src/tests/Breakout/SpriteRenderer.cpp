#include "SpriteRenderer.h"

#include "LegacyIndexBuffer.h"
#include "LegacyShader.h"
#include "Texture.h"
#include "LegacyVertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_transform.hpp"

namespace Breakout
{
    SpriteRenderer::SpriteRenderer(const std::shared_ptr<LegacyShader>& inShader)
        : m_Shader(inShader)
    {
        m_VAO = std::make_unique<LegacyVertexArray>();
        
        float vertices[] =
        {
            // Pos     UV
            0.f, 1.f,  0.f, 1.f,
            1.f, 1.f,  1.f, 1.f,
            1.f, 0.f,  1.f, 0.f,
            0.f, 0.f,  0.f, 0.f
        };

        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout{};
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        m_IBO = std::make_unique<LegacyIndexBuffer>(indices, 6);
    }

    SpriteRenderer::~SpriteRenderer()
    { }

    void SpriteRenderer::Draw(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const
    {
        m_Shader->Bind();

        glm::mat4 model{1.f};
        model = glm::translate(model, glm::vec3(position, 0.f));

        // Since our quad start on top left and we want to rotate on center, translate rotation to center
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));

        model = glm::scale(model, glm::vec3(size, 1.f));

        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniform3f("u_SpriteColor", color);

        texture.Bind(0);

        m_VAO->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
    }
}
