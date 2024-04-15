#include "LegacyFlyCameraController.h"

#include "LegacyCamera.h"
#include "GameTime.h"
#include "core/Application.h"
#include "core/Input.h"
#include "imgui/imgui.h"

// Anonymous namespace is preferred over using static for vars and methods you don't want to accidentally expose to other translation units 
namespace
{
    // Using a global var instance of "this" so we can use the instance on required global function callback
    // for glfw scroll callback
    // This is a workaround, not sure what's the best approach on these cases...
    LegacyFlyCameraController* g_CameraController;

    void ScrollCallbackWrapper(GLFWwindow* Window, double XOffset, double YOffset)
    {
        g_CameraController->UpdateCameraZoom(XOffset, YOffset);
    }
}

LegacyFlyCameraController::LegacyFlyCameraController(const std::shared_ptr<LegacyCamera>& Camera)
    : m_Camera(Camera)
{ }

void LegacyFlyCameraController::Setup(bool bEnabled)
{
    if(bEnabled)
    {
        EnableNavigation();
    }
    
    g_CameraController = this;
    glfwSetScrollCallback(Application::GetCurrentWindow(), ScrollCallbackWrapper);
}

void LegacyFlyCameraController::Shutdown()
{
    // Restore cursor default state
    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    g_CameraController = nullptr;
    glfwSetScrollCallback(Application::GetCurrentWindow(), nullptr);
}

void LegacyFlyCameraController::Update()
{
    if(m_NavigationEnabled && Input::GetKeyDown(GLFW_KEY_ESCAPE))
    {
        DisableNavigation();
    }
    else if(!m_NavigationEnabled && Input::GetKeyDown(GLFW_KEY_F))
    {
        EnableNavigation();
    }

    if(!m_NavigationEnabled)
    {
        return;
    }
    
    if(Input::GetKey(GLFW_KEY_LEFT_SHIFT))
    {
        m_Speed = m_BaseSpeed * m_FastSpeedMultiplier;
        m_Sensitivity = m_BaseSensitivity * m_FastSpeedMultiplier;
    }
    else
    {
        m_Speed = m_BaseSpeed;
        m_Sensitivity = m_BaseSensitivity;
    }
    
    UpdateCameraPosition();
    UpdateCameraRotation();
}

void LegacyFlyCameraController::EnableNavigation()
{
    m_NavigationEnabled = true;

    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set last known cursor position when entering to prevent large sudden jump on first focus
    double cursorX, cursorY;
    glfwGetCursorPos(Application::GetCurrentWindow(), &cursorX, &cursorY);

    m_CursorLastX = static_cast<float>(cursorX);
    m_CursorLastY = static_cast<float>(cursorY);
}

void LegacyFlyCameraController::DisableNavigation()
{
    m_NavigationEnabled = false;

    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void LegacyFlyCameraController::UpdateCameraPosition()
{
    float cameraSpeed = m_Speed * GameTime::DeltaTime;

    if(Input::GetKey(GLFW_KEY_W))
    {
        m_Camera->Position += cameraSpeed * m_Camera->GetForwardVector(); 
    }

    if(Input::GetKey(GLFW_KEY_S))
    {
        m_Camera->Position -= cameraSpeed * m_Camera->GetForwardVector();
    }

    if(Input::GetKey(GLFW_KEY_A))
    {
        m_Camera->Position -= m_Camera->GetRightVector() * cameraSpeed;
    }

    if(Input::GetKey(GLFW_KEY_D))
    {
        m_Camera->Position += m_Camera->GetRightVector() * cameraSpeed;
    }

    if(Input::GetKey(GLFW_KEY_E))
    {
        m_Camera->Position += m_Camera->GetUpVector() * cameraSpeed;
    }

    if(Input::GetKey(GLFW_KEY_Q))
    {
        m_Camera->Position -= m_Camera->GetUpVector() * cameraSpeed;
    }
}

void LegacyFlyCameraController::UpdateCameraRotation()
{
    double cursorX, cursorY;
    glfwGetCursorPos(Application::GetCurrentWindow(), &cursorX, &cursorY);

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

void LegacyFlyCameraController::UpdateCameraZoom(double XScrollOffset, double YScrollOffset)
{
    if(!m_NavigationEnabled)
    {
        return;
    }

    float fov = m_Camera->GetFov();
    fov -= static_cast<float>(YScrollOffset);

    m_Camera->SetFov(fov);
}

void LegacyFlyCameraController::OnImGuiRender()
{
    ImGui::Begin("Camera");
    ImGui::Text("'Esc': exit fly mode");
    ImGui::Text("'F': enter fly mode");
    ImGui::End();
}
