#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Test.h"

namespace tests
{
    class TestMenu : public Test
    {
    public:
        TestMenu(Test*& CurrentTest);
        
        void OnImGuiRender() override;

        template <typename T>
        void RegisterTest(const std::string& Name)
        {
            std::cout << "Registering test " << Name << "\n";
            
            // []() { code } is how to create a lambda in C++
            m_Tests.emplace_back(std::make_pair(Name, [](){ return new T{}; }));
        }

    private:

        // A ref to a pointer so we can change the current test and reflect on the external pointer
        Test*& m_CurrentTest;

        // Instead of having a Test* and making all of them available from start
        // We want to create them on demand, so we can better allocate and free memory
        // That's why we are holding a function that returns a Test* and accepts no params std::function<Test*()>
        // The string is to label them
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}
