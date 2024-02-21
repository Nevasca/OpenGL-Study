#include "FlyCameraController.h"

#include "Camera.h"
#include "GameTime.h"
#include "imgui/imgui.h"

// Anonymous namespace is preferred over using static for vars and methods you don't want to accidentally expose to other translation units 
namespace
{
    // Using a global var instance of "this" so we can use the instance on required global function callback
    // for glfw scroll callback
    // This is a workaround, not sure what's the best approach on these cases...
    FlyCameraController* g_CameraController;

    void ScrollCallbackWrapper(GLFWwindow* Window, double XOffset, double YOffset)
    {
        g_CameraController->UpdateCameraZoom(Window, XOffset, YOffset);
    }
}

FlyCameraController::FlyCameraController(const std::shared_ptr<Camera>& Camera)
    : m_Camera(Camera)
{ }

void FlyCameraController::Setup(GLFWwindow* Window)
{
    EnableNavigation(Window);
    
    g_CameraController = this;
    glfwSetScrollCallback(Window, ScrollCallbackWrapper);
}

void FlyCameraController::Shutdown(GLFWwindow* Window)
{
    // Restore cursor default state
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    g_CameraController = nullptr;
    glfwSetScrollCallback(Window, nullptr);
}

void FlyCameraController::ProcessInput(GLFWwindow* Window)
{
    if(m_NavigationEnabled && glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        DisableNavigation(Window);
    }
    else if(!m_NavigationEnabled && glfwGetKey(Window, GLFW_KEY_F) == GLFW_PRESS)
    {
        EnableNavigation(Window);
    }

    if(!m_NavigationEnabled)
    {
        return;
    }
    
    const bool bIsHoldingShift = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    if(bIsHoldingShift)
    {
        m_Speed = m_BaseSpeed * m_FastSpeedMultiplier;
        m_Sensitivity = m_BaseSensitivity * m_FastSpeedMultiplier;
    }
    else
    {
        m_Speed = m_BaseSpeed;
        m_Sensitivity = m_BaseSensitivity;
    }
    
    UpdateCameraPosition(Window);
    UpdateCameraRotation(Window);
}

void FlyCameraController::EnableNavigation(GLFWwindow* Window)
{
    m_NavigationEnabled = true;

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set last known cursor position when entering to prevent large sudden jump on first focus
    double cursorX, cursorY;
    glfwGetCursorPos(Window, &cursorX, &cursorY);

    m_CursorLastX = cursorX;
    m_CursorLastY = cursorY;
}

void FlyCameraController::DisableNavigation(GLFWwindow* Window)
{
    m_NavigationEnabled = false;

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void FlyCameraController::UpdateCameraPosition(GLFWwindow* Window)
{
    float cameraSpeed = m_Speed * GameTime::DeltaTime;

    if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_Camera->Position += cameraSpeed * m_Camera->GetForwardVector(); 
    }

    if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Camera->Position -= cameraSpeed * m_Camera->GetForwardVector();
    }

    if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Camera->Position -= m_Camera->GetRightVector() * cameraSpeed;
    }

    if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Camera->Position += m_Camera->GetRightVector() * cameraSpeed;
    }

    if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_Camera->Position += m_Camera->GetUpVector() * cameraSpeed;
    }

    if(glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_Camera->Position -= m_Camera->GetUpVector() * cameraSpeed;
    }
}

void FlyCameraController::UpdateCameraRotation(GLFWwindow* Window)
{
    double cursorX, cursorY;
    glfwGetCursorPos(Window, &cursorX, &cursorY);

    float xOffset = cursorX - m_CursorLastX;
    float yOffset = m_CursorLastY - cursorY; // Reversed since y-coord range from bottom to top
    m_CursorLastX = cursorX;
    m_CursorLastY = cursorY;

    xOffset *= m_Sensitivity;
    yOffset *= m_Sensitivity;

    glm::vec3 eulerRotation = m_Camera->GetRotation();
    eulerRotation.y += xOffset;
    eulerRotation.x += yOffset;

    m_Camera->SetRotation(eulerRotation);
}

void FlyCameraController::UpdateCameraZoom(GLFWwindow* Window, double XScrollOffset, double YScrollOffset)
{
    float fov = m_Camera->GetFov();
    fov -= static_cast<float>(YScrollOffset);

    m_Camera->SetFov(fov);
}

void FlyCameraController::OnImGuiRender()
{
    ImGui::Begin("Camera");
    ImGui::Text("'Esc': exit fly mode");
    ImGui::Text("'F': enter fly mode");
    ImGui::End();
}
