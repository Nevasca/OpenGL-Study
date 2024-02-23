#pragma once

#include <memory>
#include <vector>

#include "EGameState.h"
#include "GameLevel.h"

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

        std::shared_ptr<Shader> m_SpriteShader;
        std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
        std::unique_ptr<Texture> m_FaceTexture;
        std::unique_ptr<Texture> m_BackgroundTexture;
        std::shared_ptr<Texture> m_SolidBlockTexture;

        const glm::vec2 PLAYER_SIZE{100.f, 20.f};
        const float PLAYER_VELOCITY{500.f};
        std::unique_ptr<GameObject> m_Player;
        std::shared_ptr<Texture> m_PaddleTexture;
    };
}
