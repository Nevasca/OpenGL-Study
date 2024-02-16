#include "TestMenu.h"

#include "imgui/imgui.h"

namespace tests
{
    TestMenu::TestMenu(Test*& CurrentTest)
        : m_CurrentTest(CurrentTest)
    { }

    void TestMenu::OnImGuiRender()
    {
        for (auto& Test : m_Tests)
        {
            if(ImGui::Button(Test.first.c_str()))
            {
                m_CurrentTest = Test.second();
            }
        }
    }
}
