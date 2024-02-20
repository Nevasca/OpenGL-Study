#pragma once

#include <memory>
#include <GLFW/glfw3.h>

class Camera;

class FlyCameraController
{
public:

    FlyCameraController(const std::shared_ptr<Camera>& Camera);

    void Setup(GLFWwindow* Window);
    void Shutdown(GLFWwindow* Window);
    void ProcessInput(GLFWwindow* Window);
    void UpdateCameraZoom(GLFWwindow* Window, double XScrollOffset, double YScrollOffset);

private:

    std::shared_ptr<Camera> m_Camera;
    float m_CameraSpeed{2.5f};
    float m_CursorLastX{960.f / 2}; // Initialize last cursor pos on center screen
    float m_CursorLastY{540.f / 2};

    void UpdateCameraPosition(GLFWwindow* Window);
    void UpdateCameraRotation(GLFWwindow* Window);
};
