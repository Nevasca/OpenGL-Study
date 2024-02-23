#include "Game.h"

#include <memory>
#include "Core.h"
#include <GLFW/glfw3.h>

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
        m_SpriteShader = std::make_shared<Shader>("res/breakout/shaders/Sprite.shader");

        m_SpriteShader->Bind();
        m_SpriteShader->SetUniform1i("u_Image", 0);

        glm::mat4 projection = glm::ortho(0.f, static_cast<float>(Width), static_cast<float>(Height), 0.f, -1.f, 1.f);
        m_SpriteShader->SetUniformMat4f("u_Projection", projection);

        m_SpriteRenderer = std::make_unique<SpriteRenderer>(m_SpriteShader);
        
        m_FaceTexture = std::make_unique<Texture>("res/breakout/textures/awesomeface.png", true, false);
        m_BackgroundTexture = std::make_unique<Texture>("res/breakout/textures/background.jpg", false, false);
        m_SolidBlockTexture = std::make_shared<Texture>("res/breakout/textures/block_solid.png", false, false);
        m_BlockTexture = std::make_shared<Texture>("res/breakout/textures/block.png", false, false);

        GameLevel one{m_SolidBlockTexture, m_BlockTexture};
        one.Load("res/breakout/levels/01.lvl", Width, Height / 2);
        GameLevel two{m_SolidBlockTexture, m_BlockTexture};
        two.Load("res/breakout/levels/02.lvl", Width, Height / 2);
        GameLevel three{m_SolidBlockTexture, m_BlockTexture};
        three.Load("res/breakout/levels/03.lvl", Width, Height / 2);
        GameLevel four{m_SolidBlockTexture, m_BlockTexture};
        four.Load("res/breakout/levels/04.lvl", Width, Height / 2);

        Levels.reserve(4);
        Levels.emplace_back(std::move(one));        
        Levels.emplace_back(std::move(two));        
        Levels.emplace_back(std::move(three));        
        Levels.emplace_back(std::move(four));

        m_PaddleTexture = std::make_unique<Texture>("res/breakout/textures/paddle.png", true, false);
        glm::vec2 playerStartPosition = glm::vec2{Width / 2.f - PLAYER_SIZE.x / 2.f, Height - PLAYER_SIZE.y};
        m_Player = std::make_unique<GameObject>(playerStartPosition, PLAYER_SIZE, m_PaddleTexture);
    }

    void Game::ProcessInput(float deltaTime)
    {
        if(State == EGameState::GAME_ACTIVE)
        {
            float velocity = PLAYER_VELOCITY * deltaTime;

            if(Keys[GLFW_KEY_A])
            {
                if(m_Player->Position.x >= 0.f)
                {
                    m_Player->Position.x -= velocity;
                }
            }

            if(Keys[GLFW_KEY_D])
            {
                if(m_Player->Position.x < Width - PLAYER_SIZE.x)
                {
                    m_Player->Position.x += velocity;
                }
            }
        }
    }

    void Game::Update(float deltaTime)
    {
    }

    void Game::Render()
    {
        if(State == EGameState::GAME_ACTIVE)
        {
            m_SpriteRenderer->Draw(*m_BackgroundTexture, glm::vec2{0.f, 0.f}, glm::vec2{Width, Height}, 0.f);

            Levels[Level].Draw(*m_SpriteRenderer);

            m_Player->Draw(*m_SpriteRenderer);
        }
    }
}
