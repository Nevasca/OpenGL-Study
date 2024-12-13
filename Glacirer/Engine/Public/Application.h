#pragma once

#include "EngineAPI.h"

struct GLFWwindow;

namespace Glacirer
{
    class ENGINE_API Application
    {
    public:

        inline static void SetCurrentWindow(GLFWwindow* window) { m_Window = window; }
        inline static GLFWwindow* GetCurrentWindow() { return m_Window; }

    private:

        static GLFWwindow* m_Window;
    };
}
