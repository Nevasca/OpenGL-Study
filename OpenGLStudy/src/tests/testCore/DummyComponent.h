#pragma once
#include "core/GameObject/Component.h"

class DummyComponent : public Component
{
public:

    GENERATE_COMPONENT_BODY(DummyComponent)

    void Update(float deltaTime) override;
};
