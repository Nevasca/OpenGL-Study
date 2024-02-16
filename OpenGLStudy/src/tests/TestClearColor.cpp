#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace tests
{
    TestClearColor::TestClearColor()
        :m_ClearColor{0.2f, 0.3f, 0.8f, 1.f}
    { }

    TestClearColor::~TestClearColor()
    { }

    void TestClearColor::OnUpdate(float DeltaTime)
    {
        Test::OnUpdate(DeltaTime);
    }

    void TestClearColor::OnRender(const Renderer& Renderer)
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
