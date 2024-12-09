#pragma once
#include "core/GameObject/GameObject.h"

namespace tests
{
    class DummyGameObject : public GameObject
    {
        GENERATE_OBJECT_BODY(DummyGameObject)

    public:

        void Initialize() override;
        void Start() override;
        void Update(float deltaTime) override;
        void Destroy() override;
    };
}
