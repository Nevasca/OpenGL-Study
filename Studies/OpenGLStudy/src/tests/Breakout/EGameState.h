#pragma once
#include <cstdint>

namespace Breakout
{
    enum class EGameState : uint8_t
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };
}
