#pragma once
#include "EGameState.h"

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
    };
}
