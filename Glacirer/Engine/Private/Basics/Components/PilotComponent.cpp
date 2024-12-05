#include "Basics/Components/PilotComponent.h"

#include <GLFW/glfw3.h>

#include "Application.h"
#include "Input.h"
#include "Screen.h"
#include "GameObject/Transform.h"
#include <glm/common.hpp>

void PilotComponent::Initialize()
{
    Component::Initialize();

    m_CursorLastX = static_cast<float>(Screen::GetWidth()) / 2.f;
    m_CursorLastY = static_cast<float>(Screen::GetHeight()) / 2.f;
}

void PilotComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);

    UpdateSpeed();
    UpdatePosition(deltaTime);
    UpdateRotation();
}

void PilotComponent::UpdateSpeed()
{
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
}

void PilotComponent::UpdatePosition(float deltaTime)
{
    float speed = m_Speed * deltaTime;
    glm::vec3 resultPosition = GetOwnerPosition();
    
    if(Input::GetKey(GLFW_KEY_W))
    {
        resultPosition += speed * GetOwnerTransform().GetForwardVector(); 
    }

    if(Input::GetKey(GLFW_KEY_S))
    {
        resultPosition -= speed * GetOwnerTransform().GetForwardVector();
    }

    if(Input::GetKey(GLFW_KEY_A))
    {
        resultPosition -= speed * GetOwnerTransform().GetRightVector();
    }

    if(Input::GetKey(GLFW_KEY_D))
    {
        resultPosition += speed * GetOwnerTransform().GetRightVector();
    }

    if(Input::GetKey(GLFW_KEY_E))
    {
        resultPosition += speed * m_WorldUp;
    }

    if(Input::GetKey(GLFW_KEY_Q))
    {
        resultPosition -= speed * m_WorldUp;
    }

    SetOwnerPosition(resultPosition);
}

void PilotComponent::UpdateRotation()
{
    double cursorX, cursorY;
    glfwGetCursorPos(Application::GetCurrentWindow(), &cursorX, &cursorY);

    float xOffset = m_CursorLastX - static_cast<float>(cursorX);
    float yOffset = m_CursorLastY - static_cast<float>(cursorY); 
    m_CursorLastX = static_cast<float>(cursorX);
    m_CursorLastY = static_cast<float>(cursorY);

    xOffset *= m_Sensitivity;
    yOffset *= m_Sensitivity;

    glm::vec3 eulerRotation = GetOwnerRotation();
    eulerRotation.y += xOffset;
    eulerRotation.x += yOffset;

    if(m_ClampPitchEnabled)
    {
        eulerRotation.x = glm::clamp(eulerRotation.x, m_MinPitch, m_MaxPitch);  
    }
    
    if(m_ClampYawEnabled)
    {
        eulerRotation.y = glm::clamp(eulerRotation.y, m_MinYaw, m_MaxYaw);
    }

    SetOwnerRotation(eulerRotation);
}

void PilotComponent::Enable()
{
    Component::Enable();

    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set last known cursor position when entering to prevent large sudden jump on first focus
    double cursorX, cursorY;
    glfwGetCursorPos(Application::GetCurrentWindow(), &cursorX, &cursorY);

    m_CursorLastX = static_cast<float>(cursorX);
    m_CursorLastY = static_cast<float>(cursorY);
}

void PilotComponent::Disable()
{
    Component::Disable();

    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
