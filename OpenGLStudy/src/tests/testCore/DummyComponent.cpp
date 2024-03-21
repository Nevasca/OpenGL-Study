#include "DummyComponent.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "core/Input.h"

void DummyComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);

    if(Input::GetKeyDown(GLFW_KEY_H))
    {
        std::cout << "Hello World!\n";
    }
}
