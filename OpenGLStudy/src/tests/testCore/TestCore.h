#pragma once

#include "tests/Test.h"
#include "core/World.h"
#include "DummyGameObject.h"

namespace tests
{
    class TestCore : public Test
    {
    public:
        TestCore();
        ~TestCore() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<World> m_World{};
        std::shared_ptr<DummyGameObject> m_DummyObjectA;
        std::shared_ptr<DummyGameObject> m_DummyObjectB;

        void SpawnLights(GameObject& camera);
    };
}
