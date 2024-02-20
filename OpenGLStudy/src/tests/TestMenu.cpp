#include "TestMenu.h"

#include "imgui/imgui.h"

namespace tests
{
    TestMenu::TestMenu(Test*& CurrentTest, GLFWwindow& CurrentWindow)
        : m_CurrentTest(CurrentTest), m_CurrentWindow(CurrentWindow)
    { }

    void TestMenu::OnImGuiRender()
    {
        for (auto& Test : m_Tests)
        {
            if(ImGui::Button(Test.first.c_str()))
            {
                m_CurrentTest = Test.second();
                m_CurrentTest->Setup(&m_CurrentWindow);
            }
        }
    }
}
