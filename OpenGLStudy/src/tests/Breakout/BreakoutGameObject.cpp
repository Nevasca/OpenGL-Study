#include "BreakoutGameObject.h"

#include "SpriteRenderer.h"

namespace Breakout
{
    BreakoutGameObject::BreakoutGameObject()
    { }

    BreakoutGameObject::BreakoutGameObject(glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite, glm::vec3 color, glm::vec2 velocity)
        : Position(position), Size(size), Velocity(velocity), Color(color), Sprite(sprite) 
    {
    }

    void BreakoutGameObject::Draw(const SpriteRenderer& Renderer) const
    {
        Renderer.Draw(*Sprite, Position, Size, Rotation, Color);
    }
}
