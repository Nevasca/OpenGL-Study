#pragma once

#include <memory>

#include "EGameState.h"

class Texture;
class Shader;

namespace Breakout
{
    class SpriteRenderer;
}

namespace Breakout
{
    class Game
    {
    public:
        EGameState State;
        bool Keys[1024];
        unsigned int Width;
        unsigned Height;

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
    };
}
