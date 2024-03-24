#include "TestMenu.h"

#include <GLFW/glfw3.h>

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
                double startLoadTime = glfwGetTime();
                m_CurrentTest = Test.second();
                m_CurrentTest->Setup(&m_CurrentWindow);
                const double loadTimeInSeconds = glfwGetTime() - startLoadTime;

                std::cout << "Test " << Name << "loaded in " << loadTimeInSeconds << "s";

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
                double startLoadTime = glfwGetTime();
                m_CurrentTest = Test.second();
                m_CurrentTest->Setup(&m_CurrentWindow);

                const double loadTimeInSeconds = glfwGetTime() - startLoadTime;

                std::cout << "Test " << Test.first << " loaded in " << loadTimeInSeconds << "s";
            }
        }
    }
}
