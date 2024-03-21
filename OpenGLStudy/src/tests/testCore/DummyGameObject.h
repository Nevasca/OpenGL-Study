#pragma once
#include "core/GameObject/GameObject.h"

namespace tests
{
    class DummyGameObject : public GameObject
    {
    public:

        void Initialize() override;
        void Start() override;
        void Update(float deltaTime) override;
        void Destroy() override;
    };
}
