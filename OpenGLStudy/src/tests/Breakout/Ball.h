#pragma once
#include "GameObject.h"

namespace Breakout
{
    class Ball : public GameObject
    {
    public:

        float Radius{12.f};
        bool Stuck{true};

        Ball(glm::vec2 position, float radius, glm::vec2 velocity, const std::shared_ptr<Texture>& sprite);

        glm::vec2 Move(float deltaTime, unsigned int windowWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);
    };
}
