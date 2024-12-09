#pragma once
#include "core/GameObject/Component.h"

class DummyComponent : public Component
{
    GENERATE_COMPONENT_BODY(DummyComponent)

public:

    void Update(float deltaTime) override;
};
