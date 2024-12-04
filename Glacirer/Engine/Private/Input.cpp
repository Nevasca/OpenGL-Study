#include "Input.h"

#include <GLFW/glfw3.h>

bool Input::m_Keys[2][KEYS_LENGTH] = {};
bool* Input::m_CurrentKeys = m_Keys[1];
bool* Input::m_PreviousKeys = m_Keys[0];

void Input::Process(GLFWwindow* window)
{
    for(int i = 0; i < KEYS_LENGTH; i++)
    {
        m_CurrentKeys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }
}

bool Input::GetKey(int keyCode)
{
    return m_CurrentKeys[keyCode];
}

bool Input::GetKeyDown(int keyCode)
{
    return m_CurrentKeys[keyCode] && !m_PreviousKeys[keyCode];
}

bool Input::GetKeyUp(int keyCode)
{
    return !m_CurrentKeys[keyCode] && m_PreviousKeys[keyCode];
}

void Input::SwapBuffers()
{
    bool* temp = m_CurrentKeys;
    m_CurrentKeys = m_PreviousKeys;
    m_PreviousKeys = temp;
}
