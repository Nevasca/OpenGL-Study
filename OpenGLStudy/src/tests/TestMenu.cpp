#include "TestMenu.h"

#include "imgui/imgui.h"

namespace tests
{
    TestMenu::TestMenu(Test*& CurrentTest, GLFWwindow& CurrentWindow)
        : m_CurrentTest(CurrentTest), m_CurrentWindow(CurrentWindow)
    { }

    void TestMenu::OpenTest(const std::string& Name)
    {
        for(auto& Test : m_Tests)
        {
            if(Test.first == Name)
            {
                m_CurrentTest = Test.second();
                m_CurrentTest->Setup(&m_CurrentWindow);

                return;
            }
        }

        std::cout << "Test " << Name << " not found!\n";
    }

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
