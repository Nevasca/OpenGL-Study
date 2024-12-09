#include "DummyComponent.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "core/Input.h"
#include "glm/vec3.hpp"

void DummyComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);

    constexpr float speed = 10;

    if(Input::GetKey(GLFW_KEY_A))
    {
        glm::vec3 position = GetOwnerPosition();
        position.x -= speed * deltaTime;
        SetOwnerPosition(position);
    }

    if(Input::GetKey(GLFW_KEY_D))
    {
        glm::vec3 position = GetOwnerPosition();
        position.x += speed * deltaTime;
        SetOwnerPosition(position);
    }
}
