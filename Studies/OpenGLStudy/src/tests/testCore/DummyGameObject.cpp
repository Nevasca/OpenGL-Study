#include "DummyGameObject.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "core/Input.h"

namespace tests
{
    void DummyGameObject::Initialize()
    {
        GameObject::Initialize();

        std::cout << "Dummy Initialize\n";
    }

    void DummyGameObject::Start()
    {
        GameObject::Start();

        std::cout << "Dummy Start\n";
    }

    void DummyGameObject::Update(float deltaTime)
    {
        GameObject::Update(deltaTime);

        if(Input::GetKeyDown(GLFW_KEY_E))
        {
            const glm::vec3 position = GetPosition();
            std::cout << "Eai, beleza? To no (" << position.x << ", " << position.y << ", " << position.z << ")\n";
        }
    }

    void DummyGameObject::Destroy()
    {
        GameObject::Destroy();

        std::cout << "Dummy destroy\n";
    }
}
