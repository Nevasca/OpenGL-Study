#pragma once

#include "EngineAPI.h"

struct GLFWwindow;

namespace Glacirer
{
    class ENGINE_API Input
    {
    public:

        static constexpr int KEYS_LENGTH = 1024; 

        static void Process(GLFWwindow* window);
        static bool GetKey(int keyCode);
        static bool GetKeyDown(int keyCode);
        static bool GetKeyUp(int keyCode);
        static void SwapBuffers();

    private:
    
        static bool m_Keys[2][KEYS_LENGTH];
        static bool* m_CurrentKeys;
        static bool* m_PreviousKeys;
    };
}
