#include "Game.h"

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
    }

    void Game::ProcessInput(float deltaTime)
    {
    }

    void Game::Update(float deltaTime)
    {
    }

    void Game::Render()
    {
    }
}
