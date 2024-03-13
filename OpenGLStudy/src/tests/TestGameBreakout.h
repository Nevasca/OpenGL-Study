#pragma once

#include "Test.h"
#include "Breakout/Game.h"

namespace tests
{
    class TestGameBreakout : public Test
    {
    public:

        
        TestGameBreakout();
        ~TestGameBreakout() override;
        
        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        const unsigned int SCREEN_WIDTH = 800;
        const unsigned int SCREEN_HEIGHT = 600;

        Breakout::Game m_Breakout{SCREEN_WIDTH, SCREEN_HEIGHT};

        int m_previousScreenWidth{960};
        int m_previousScreenHeight{540};
    };
}
