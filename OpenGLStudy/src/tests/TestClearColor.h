#pragma once
#include "Test.h"

namespace tests
{
    class TestClearColor : public Test
    {
    public:

        TestClearColor();
        ~TestClearColor() override;
        
        void OnUpdate(float DeltaTime) override;
        void OnRender(const Renderer& Renderer) override;
        void OnImGuiRender() override;

    private:

        float m_ClearColor[4];
    };
}
