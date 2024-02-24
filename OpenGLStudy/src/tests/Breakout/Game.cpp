#include "Game.h"

#include <memory>
#include <GLFW/glfw3.h>

#include "Ball.h"
#include "ParticleGenerator.h"
#include "Shader.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_common.hpp"

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
        
        m_FaceTexture = std::make_shared<Texture>("res/breakout/textures/awesomeface.png", true, false);
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
        glm::vec2 playerStartPosition = GetPlayerStartPosition();
        m_Player = std::make_unique<GameObject>(playerStartPosition, PLAYER_SIZE, m_PaddleTexture);

        glm::vec2 ballStartPosition = GetBallStartPosition(playerStartPosition);
        
        m_Ball = std::make_unique<Ball>(ballStartPosition, BALL_RADIUS, BALL_INITIAL_VELOCITY, m_FaceTexture);

        m_ParticleShader = std::make_unique<Shader>("res/breakout/shaders/Particle.shader");
        m_ParticleShader->Bind();
        m_ParticleShader->SetUniformMat4f("u_Projection", projection);
        m_ParticleShader->SetUniform1i("u_Sprite", 0);

        m_ParticleTexture = std::make_unique<Texture>("res/breakout/textures/particle.png", true, false);

        m_Particles = std::make_unique<ParticleGenerator>(m_ParticleShader, m_ParticleTexture, 500);
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
                    if(m_Ball->Stuck)
                    {
                        m_Ball->Position.x -= velocity;
                    }
                }
            }

            if(Keys[GLFW_KEY_D])
            {
                if(m_Player->Position.x < Width - PLAYER_SIZE.x)
                {
                    m_Player->Position.x += velocity;
                    if(m_Ball->Stuck)
                    {
                        m_Ball->Position.x += velocity;
                    }
                }
            }

            if(Keys[GLFW_KEY_SPACE])
            {
                m_Ball->Stuck = false;
            }
        }
    }

    void Game::Update(float deltaTime)
    {
        m_Ball->Move(deltaTime, Width);

        m_Particles->Update(deltaTime, *m_Ball, 2, glm::vec2(m_Ball->Radius / 2.f));

        DoCollisions();

        if(m_Ball->Position.y > Height)
        {
            ResetLevel();
            ResetPlayer();
        }
    }

    void Game::Render()
    {
        if(State == EGameState::GAME_ACTIVE)
        {
            m_SpriteRenderer->Draw(*m_BackgroundTexture, glm::vec2{0.f, 0.f}, glm::vec2{Width, Height}, 0.f);

            Levels[Level].Draw(*m_SpriteRenderer);

            m_Player->Draw(*m_SpriteRenderer);

            m_Particles->Draw();
            
            m_Ball->Draw(*m_SpriteRenderer);
        }
    }

    void Game::DoCollisions()
    {
        for(GameObject& brick : Levels[Level].Bricks)
        {
            if(brick.Destroyed)
            {
                continue;
            }

            Collision collision = CheckCollision(*m_Ball, brick);
            bool bHasCollided = std::get<0>(collision);

            if(!bHasCollided)
            {
                continue;
            }

            if(!brick.IsSolid)
            {
                brick.Destroyed = true;
            }

            // Collision resolution
            EDirection direction = std::get<1>(collision);
            glm::vec2 difference = std::get<2>(collision);

            if(direction == LEFT || direction == RIGHT)
            {
                // reverse horizontal velocity
                m_Ball->Velocity.x = -m_Ball->Velocity.x;

                float penetration = m_Ball->Radius - std::abs(difference.x);

                // If ball is trying to move towards the left direction and it collided,
                // it means we need to move it to the right to not overlap
                // and so on for other directions
                if(direction == LEFT)
                {
                    m_Ball->Position.x += penetration; // move ball to the right
                }
                else
                {
                    m_Ball->Position.x -= penetration; // move ball to the left
                }
            }
            else
            {
                // reverse vertical velocity
                m_Ball->Velocity.y = -m_Ball->Velocity.y;

                float penetration = m_Ball->Radius - std::abs(difference.y);

                if(direction == UP)
                {
                    m_Ball->Position.y -= penetration;
                }
                else
                {
                    m_Ball->Position.y += penetration;
                }
            }
        }

        Collision result = CheckCollision(*m_Ball, *m_Player);
        bool bHasCollidedWithPaddle = std::get<0>(result);
        
        if(m_Ball->Stuck || !bHasCollidedWithPaddle)
        {
            return;
        }

        // Check how far from center of the paddle the ball has collided
        float centerPaddle = m_Player->Position.x + m_Player->Size.x / 2.f;
        float distance = m_Ball->Position.x + m_Ball->Radius - centerPaddle;
        float percentage = distance / (m_Player->Size.x / 2.f);

        float strength = 2.f;
        glm::vec2 oldVelocity = m_Ball->Velocity;
        m_Ball->Velocity.x = BALL_INITIAL_VELOCITY.x * percentage * strength;
        // m_Ball->Velocity.y = -m_Ball->Velocity.y; // When paddle moves with high velocity, ball may end up inside paddle, causing stick paddle
        m_Ball->Velocity.y = -1.f * abs(m_Ball->Velocity.y); // Hack to reduce stick paddle 
        m_Ball->Velocity = glm::normalize(m_Ball->Velocity) * glm::length(oldVelocity);
    }

    bool Game::CheckCollision(const GameObject& one, const GameObject& two) const //AABB - AABB collision
    {
        // Position.xy is top left
        
        bool bIsOverlappingX = one.Position.x + one.Size.x >= two.Position.x &&
            two.Position.x + two.Size.x >= one.Position.x;

        bool bIsOverlappingY = one.Position.y + one.Size.y >= two.Position.y &&
            two.Position.y + two.Size.y >= one.Position.y;

        // Only if both axis overlaps a collision is occuring
        return bIsOverlappingX && bIsOverlappingY;
    }

    Collision Game::CheckCollision(const Ball& one, const GameObject& two) const //AABB - Circle collision
    {
        glm::vec2 circleCenter = one.Position + one.Radius;

        glm::vec2 aabbHalfExtents = two.Size / 2.f;
        glm::vec2 aabbCenter = two.Position + aabbHalfExtents;

        glm::vec2 centerDifference = circleCenter - aabbCenter;
        glm::vec2 clampedDifference = glm::clamp(centerDifference, -aabbHalfExtents, aabbHalfExtents);
        glm::vec2 closestPoint = aabbCenter + clampedDifference;

        centerDifference = closestPoint - circleCenter;

        if(glm::length(centerDifference) < one.Radius)
        {
            return std::make_tuple(true, GetClosestDirection(centerDifference), centerDifference);
        }

        return std::make_tuple(false, UP, glm::vec2(0.f, 0.f)); 
    }

    EDirection Game::GetClosestDirection(glm::vec2 direction) const
    {
        glm::vec2 compass[] = {
            glm::vec2(0.f, 1.f), // up
            glm::vec2(1.0, 0.f), // right
            glm::vec2(0.f, -1.f), // down
            glm::vec2(-1.f, 0.f), // left
        };

        float max = 0.f;
        unsigned int bestMatch = -1;
        for(unsigned int i = 0; i < 4; i++)
        {
            float dotProduct = glm::dot(glm::normalize(direction), compass[i]);
            if(dotProduct > max)
            {
                max = dotProduct;
                bestMatch = i;
            }
        }

        return static_cast<EDirection>(bestMatch);
    }

    void Game::ResetLevel()
    {
        Levels[Level].Reset();
    }

    void Game::ResetPlayer()
    {
        m_Player->Position = GetPlayerStartPosition();
        m_Ball->Reset(GetBallStartPosition(m_Player->Position), BALL_INITIAL_VELOCITY);
    }

    glm::vec2 Game::GetPlayerStartPosition()
    {
        return glm::vec2{Width / 2.f - PLAYER_SIZE.x / 2.f, Height - PLAYER_SIZE.y};
    }

    glm::vec2 Game::GetBallStartPosition(const glm::vec2& playerPosition)
    {
        return playerPosition + glm::vec2{PLAYER_SIZE.x / 2.f - BALL_RADIUS, -BALL_RADIUS * 2.f};
    }
}
