#include "ParticleGenerator.h"

#include "BreakoutGameObject.h"

#include "VertexBufferLayout.h"

namespace Breakout
{
    ParticleGenerator::ParticleGenerator(const std::shared_ptr<LegacyShader>& shader, const std::shared_ptr<Texture>& texture, unsigned amount)
        : m_Shader(shader), m_Texture(texture), m_Amount(amount)
    {
        Init();
    }

    void ParticleGenerator::Init()
    {
        m_VAO = std::make_unique<VertexArray>();
        
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

        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        m_Particles.reserve(m_Amount);
        for(unsigned int i = 0; i < m_Amount; i++)
        {
            m_Particles.emplace_back();
        }
    }

    void ParticleGenerator::Update(float deltaTime, BreakoutGameObject& objectAttached, unsigned int newParticles, glm::vec2 offset)
    {
        // Add new particles
        for(unsigned int i = 0; i < newParticles; i++)
        {
            unsigned int unusedParticle = FindFirstUnusedParticle();
            RespawnParticle(m_Particles[unusedParticle], objectAttached, offset);
        }
    
        // Update all particle
        for(unsigned int i = 0; i < m_Amount; i++)
        {
            Particle& particle = m_Particles[i];
            particle.Life -= deltaTime;
    
            // Update alive particles
            if(particle.Life > 0.f)
            {
                particle.Position -= particle.Velocity * deltaTime;
                particle.Color.a -= 2.5f * deltaTime;
            }
        }
    }

    void ParticleGenerator::Draw() const
    {
        // Set to additive blend to give a 'glow' effect
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

        m_Shader->Bind();
        m_Texture->Bind(0);

        for(const Particle& particle : m_Particles)
        {
            if(particle.Life <= 0.f)
            {
                continue;
            }

            m_Shader->SetUniform2f("u_Offset", particle.Position);
            m_Shader->SetUniform4f("u_Color", particle.Color);

            m_VAO->Bind();

            GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
        }

        // Set to default blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    unsigned int ParticleGenerator::FindFirstUnusedParticle()
    {
        // First search form last used particle, usually return almost instantly
        for(unsigned int i = m_LastUsedParticle; i < m_Amount; i++)
        {
            if(m_Particles[i].Life <= 0.f)
            {
                m_LastUsedParticle = i;
                return i;
            }
        }

        // Otherwise, do a linear search
        for(unsigned int i = 0; i < m_Amount; i++)
        {
            if(m_Particles[i].Life <= 0.f)
            {
                m_LastUsedParticle = i;
                return i;
            }
        }

        // All particles in use, override first one (if this occurs frequently, more particles should be reserved)
        m_LastUsedParticle = 0;
        return 0;
    }

    void ParticleGenerator::RespawnParticle(Particle& particle, const BreakoutGameObject& objectAttached, glm::vec2 offset)
    {
        float random = (rand() % 100 - 50) / 10.f;
        float randomColor = 0.5f + (rand() % 100) / 100.f;
    
        particle.Position = objectAttached.Position + random + offset;
        particle.Color = glm::vec4(randomColor, randomColor, randomColor, 1.f);
        particle.Life = 1.f;
        particle.Velocity = objectAttached.Velocity * 0.1f;
    }
}
