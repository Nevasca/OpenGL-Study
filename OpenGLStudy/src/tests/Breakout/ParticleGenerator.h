#pragma once
#include <memory>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "LegacyIndexBuffer.h"
#include "LegacyShader.h"
#include "Texture.h"
#include "LegacyVertexArray.h"
#include "VertexBuffer.h"

namespace Breakout
{
    class BreakoutGameObject;

    struct Particle
    {
        glm::vec2 Position{0.f};
        glm::vec2 Velocity{0.f};
        glm::vec4 Color{1.f};
        float Life{0.f};
    };
    
    class ParticleGenerator
    {
    public:

        ParticleGenerator(const std::shared_ptr<LegacyShader>& shader, const std::shared_ptr<Texture>& texture, unsigned int amount);

        void Update(float deltaTime, BreakoutGameObject& objectAttached, unsigned int newParticles, glm::vec2 offset = glm::vec2{0.f});
        void Draw() const;

    private:

        std::unique_ptr<LegacyVertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<LegacyIndexBuffer> m_IBO;
        std::shared_ptr<LegacyShader> m_Shader;
        std::shared_ptr<Texture> m_Texture;

        std::vector<Particle> m_Particles{};
        unsigned int m_Amount{0};
        unsigned int m_LastUsedParticle{0};

        void Init();
        unsigned int FindFirstUnusedParticle();
        void RespawnParticle(Particle& particle, const BreakoutGameObject& objectAttached, glm::vec2 offset = glm::vec2{0.f});
    };
}
