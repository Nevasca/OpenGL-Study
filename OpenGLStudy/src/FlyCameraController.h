#pragma once

#include <memory>
#include <GLFW/glfw3.h>

class Camera;

class FlyCameraController
{
public:

    FlyCameraController(const std::shared_ptr<Camera>& Camera);

    void Setup(GLFWwindow* Window, bool bEnable = true);
    void Shutdown(GLFWwindow* Window);
    void ProcessInput(GLFWwindow* Window);
    void UpdateCameraZoom(GLFWwindow* Window, double XScrollOffset, double YScrollOffset);
    void OnImGuiRender();

private:

    std::shared_ptr<Camera> m_Camera;
    bool m_NavigationEnabled{false};
    float m_BaseSpeed{2.5f};
    float m_Speed{2.5f};
    float m_BaseSensitivity{0.1f};
    float m_Sensitivity{0.1f};
    float m_FastSpeedMultiplier{3.f};
    float m_CursorLastX{960.f / 2}; // Initialize last cursor pos on center screen
    float m_CursorLastY{540.f / 2};

    void EnableNavigation(GLFWwindow* Window);
    void DisableNavigation(GLFWwindow* Window);
    void UpdateCameraPosition(GLFWwindow* Window);
    void UpdateCameraRotation(GLFWwindow* Window);
};
