#include "Ball.h"

namespace Breakout
{
    Ball::Ball(glm::vec2 position, float radius, glm::vec2 velocity, const std::shared_ptr<LegacyTexture>& sprite)
        : BreakoutGameObject(position, glm::vec2{radius * 2.f}, sprite, glm::vec3{1.f}, velocity)
    { }

    glm::vec2 Ball::Move(float deltaTime, unsigned windowWidth)
    {
        if(Stuck)
        {
            return Position;
        }

        Position += Velocity * deltaTime;

        // Check window bounds, reverse velocity and restore correct position
        if(Position.x <= 0.f)
        {
            Velocity.x = -Velocity.x;
            Position.x = 0.f;
        }
        else if(Position.x + Size.x >= windowWidth)
        {
            Velocity.x = -Velocity.x;
            Position.x = windowWidth - Size.x;
        }
        else if(Position.y <= 0.f)
        {
            Velocity.y = - Velocity.y;
            Position.y = 0.f;
        }
    }

    void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
    {
        Position = position;
        Velocity = velocity;
        Stuck = true;
    }
}
