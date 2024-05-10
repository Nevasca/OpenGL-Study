#pragma once

#include <memory>

#include "Test.h"
#include "core/World.h"
#include "editor/EngineEditor.h"

namespace tests
{
    class TestCore : public Test
    {
    public:

        TestCore();
        virtual ~TestCore() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    protected:

        std::unique_ptr<World> m_World{};
        std::unique_ptr<Editor::EngineEditor> m_Editor{};

    private:

        void SpawnDefaultWorld();
    };
}
