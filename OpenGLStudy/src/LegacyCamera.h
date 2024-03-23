#pragma once
#include "glm/glm.hpp"

class LegacyCamera
{
public:

    LegacyCamera(float ScreenWidth, float ScreenHeight);

    glm::vec3 Position{0.f, 0.f, 3.f};

    glm::vec3 GetForwardVector() const;
    glm::vec3 GetUpVector() const;
    glm::vec3 GetRightVector() const;
    glm::vec3 GetRotation() const;
    void SetRotation(const glm::vec3& EulerRotation);
    float GetFov() const;
    void SetFov(float Fov);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;
    
private:

    glm::mat4 m_Projection;
    
    glm::vec3 m_Forward{0.f, 0.f, -1.f};
    glm::vec3 m_Up{0.f, 1.f, 0.f};
    
    glm::vec3 m_Rotation{0.f, -90.f, 0.f}; // Makes the camera points towards negative z-axis by default
    bool m_ClampPitchEnabled{true};
    float m_MinPitch{-89.f};
    float m_MaxPitch{89.f};
    bool m_ClampYawEnabled{false};
    float m_MinYaw{-179.f};
    float m_MaxYaw{179.f};

    float m_ScreenWidth{960.f};
    float m_ScreenHeight{960.f};
    float m_Fov{45.f};
    float m_MinFov{1.f};
    float m_MaxFov{90.f};
    float m_NearPlane{0.1f};
    float m_FarPlane{100.f};
};
