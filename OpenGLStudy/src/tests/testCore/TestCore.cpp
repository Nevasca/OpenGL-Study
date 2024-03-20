#include "TestCore.h"

namespace tests
{
    TestCore::TestCore()
    {
        m_World.Initialize();
        m_DummyObjectA = m_World.Spawn<DummyGameObject>();
        m_DummyObjectB = m_World.Spawn<DummyGameObject>(glm::vec3{10.f, 1.f, 2.f});
    }

    TestCore::~TestCore()
    {
        m_World.Shutdown();
    }

    void TestCore::OnUpdate(float DeltaTime)
    {
        m_World.Update(DeltaTime);
    }

    void TestCore::OnRender()
    {
        m_World.Render();
    }

    void TestCore::OnImGuiRender()
    {
        
    }
}
