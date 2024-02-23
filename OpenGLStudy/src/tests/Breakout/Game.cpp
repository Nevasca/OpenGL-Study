#include "Game.h"

#include <memory>

#include "Shader.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Breakout
{
    Game::Game(unsigned width, unsigned height)
        : State(EGameState::GAME_ACTIVE), Keys{}, Width(width), Height(height)
    {
        
    }

    Game::~Game()
    {
    }

    void Game::Init()
    {
        m_SpriteShader = std::make_shared<Shader>("res/shaders/breakout/Sprite.shader");

        m_SpriteShader->Bind();
        m_SpriteShader->SetUniform1i("u_Image", 0);

        glm::mat4 projection = glm::ortho(0.f, static_cast<float>(Width), static_cast<float>(Height), 0.f, -1.f, 1.f);
        m_SpriteShader->SetUniformMat4f("u_Projection", projection);

        m_SpriteRenderer = std::make_unique<SpriteRenderer>(m_SpriteShader);
        
        m_FaceTexture = std::make_unique<Texture>("res/textures/breakout/awesomeface.png", false, true);
    }

    void Game::ProcessInput(float deltaTime)
    {
    }

    void Game::Update(float deltaTime)
    {
    }

    void Game::Render()
    {
        m_SpriteRenderer->Draw(*m_FaceTexture, glm::vec2(200.f, 200.f), glm::vec2(300.f, 400.f), 45.f, glm::vec3(0.f, 1.f, 0.f));
    }
}
