#pragma once

#include "tests/Test.h"
#include "core/World.h"
#include "DummyGameObject.h"
#include "editor/EngineEditor.h"

namespace tests
{
    class TestCoreSandbox : public Test
    {
    public:
    
        TestCoreSandbox();
        ~TestCoreSandbox() override;

        void Setup(GLFWwindow* Window) override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<World> m_World{};
        std::unique_ptr<Editor::EngineEditor> m_Editor{};
        std::shared_ptr<DummyGameObject> m_DummyObjectA;
        std::shared_ptr<DummyGameObject> m_DummyObjectB;

        void SpawnTransparentObjects();
        void SpawnLights(GameObject& camera);
        void SpawnCrates();
        void SpawnFloor();
        void SpawnBridge();
    };
}
