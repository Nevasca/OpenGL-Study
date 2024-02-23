#pragma once

#include <memory>
#include <vector>

#include "EGameState.h"
#include "GameLevel.h"

namespace Breakout
{
    class Ball;
}

class Texture;
class Shader;
class SpriteRenderer;

namespace Breakout
{
    class Game
    {
    public:
        EGameState State{EGameState::GAME_ACTIVE};
        bool Keys[1024];
        unsigned int Width;
        unsigned Height;

        std::vector<GameLevel> Levels;
        unsigned int Level{0};
        std::shared_ptr<Texture> m_BlockTexture;

        Game(unsigned width, unsigned height);
        ~Game();

        void Init();
        void ProcessInput(float deltaTime);
        void Update(float deltaTime);
        void Render();

    private:

        const glm::vec2 PLAYER_SIZE{100.f, 20.f};
        const float PLAYER_VELOCITY{500.f};
        const glm::vec2 BALL_INITIAL_VELOCITY{100.f, -350.f};
        const float BALL_RADIUS{12.5f};
        
        std::shared_ptr<Shader> m_SpriteShader;
        std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
        std::unique_ptr<Texture> m_BackgroundTexture;
        std::shared_ptr<Texture> m_SolidBlockTexture;

        std::unique_ptr<GameObject> m_Player;
        std::shared_ptr<Texture> m_PaddleTexture;

        std::unique_ptr<Ball> m_Ball;
        std::shared_ptr<Texture> m_FaceTexture;
    };
}
