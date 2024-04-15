#pragma once

#include <memory>
#include <GLFW/glfw3.h>

class LegacyCamera;

class LegacyFlyCameraController
{
public:

    LegacyFlyCameraController(const std::shared_ptr<LegacyCamera>& Camera);

    void Setup(bool bEnable = true);
    void Shutdown();
    void Update();
    void UpdateCameraZoom(double XScrollOffset, double YScrollOffset);
    void OnImGuiRender();

private:

    std::shared_ptr<LegacyCamera> m_Camera;
    bool m_NavigationEnabled{false};
    float m_BaseSpeed{2.5f};
    float m_Speed{2.5f};
    float m_BaseSensitivity{0.1f};
    float m_Sensitivity{0.1f};
    float m_FastSpeedMultiplier{3.f};
    float m_CursorLastX{960.f / 2}; // Initialize last cursor pos on center screen
    float m_CursorLastY{540.f / 2};

    void EnableNavigation();
    void DisableNavigation();
    void UpdateCameraPosition();
    void UpdateCameraRotation();
};
