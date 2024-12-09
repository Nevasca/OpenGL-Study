#pragma once

struct GLFWwindow;

class Application
{
public:

    inline static void SetCurrentWindow(GLFWwindow* window) { m_Window = window; }
    inline static GLFWwindow* GetCurrentWindow() { return m_Window; }

private:

    static GLFWwindow* m_Window;
};
