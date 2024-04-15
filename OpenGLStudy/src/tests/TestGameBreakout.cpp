#include "OpenGLCore.h"
#include <GLFW/glfw3.h>

#include "TestGameBreakout.h"

#include "core/GameTime.h"

namespace tests
{
    namespace
    {
        Breakout::Game* g_Game;

        void HandleWindowResized(GLFWwindow* Window, int Width, int Height)
        {
            GLCall(glViewport(0, 0, Width, Height));
        }

        void HandleKey(GLFWwindow* Window, int Key, int Scancode, int Action, int Mode)
        {
            if(Key >= 0 && Key < 1024)
            {
                if(Action == GLFW_PRESS)
                {
                    g_Game->Keys[Key] = true;
                }
                else if(Action == GLFW_RELEASE)
                {
                    g_Game->Keys[Key] = false;
                }
            }
        }
    }
    
    TestGameBreakout::TestGameBreakout()
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    TestGameBreakout::~TestGameBreakout()
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void TestGameBreakout::Setup(GLFWwindow* Window)
    {
        g_Game = &m_Breakout;

        glfwGetWindowSize(Window, &m_previousScreenWidth, &m_previousScreenHeight);
        glfwSetWindowSize(Window, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT));

        GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

        glfwSetFramebufferSizeCallback(Window, HandleWindowResized);
        glfwSetKeyCallback(Window, HandleKey);

        m_Breakout.Init();
    }

    void TestGameBreakout::Shutdown(GLFWwindow* Window)
    {
        g_Game = nullptr;

        glfwSetWindowSize(Window, m_previousScreenWidth, m_previousScreenHeight);

        GLCall(glViewport(0, 0, m_previousScreenWidth, m_previousScreenHeight));

        glfwSetFramebufferSizeCallback(Window, nullptr);
        glfwSetKeyCallback(Window, nullptr);
    }

    void TestGameBreakout::OnUpdate(float DeltaTime)
    {
        m_Breakout.ProcessInput(GameTime::DeltaTime);
        m_Breakout.Update(DeltaTime);
    }

    void TestGameBreakout::OnRender()
    {
        m_Breakout.Render();
    }

    void TestGameBreakout::OnImGuiRender()
    { }
}
