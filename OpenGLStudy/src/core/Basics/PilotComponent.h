#pragma once
#include "core/GameObject/Component.h"

class PilotComponent : public Component
{
    GENERATE_COMPONENT_BODY(PilotComponent)
    
public:

    void Initialize() override;
    void Update(float deltaTime) override;
    void Enable() override;
    void Disable() override;

    void SetClampPitchEnabled(const bool bEnabled) { m_ClampPitchEnabled = bEnabled; }
    void SetClampYawEnabled(const bool bEnabled) { m_ClampYawEnabled = bEnabled; }

private:

    float m_BaseSpeed{2.5f};
    float m_Speed{2.5f};
    float m_BaseSensitivity{0.1f};
    float m_Sensitivity{0.1f};
    float m_FastSpeedMultiplier{3.f};
    float m_CursorLastX{960.f / 2.f};
    float m_CursorLastY{540.f / 2.f};
    bool m_ClampPitchEnabled{true};
    float m_MinPitch{-89.f};
    float m_MaxPitch{89.f};
    bool m_ClampYawEnabled{false};
    float m_MinYaw{-179.f};
    float m_MaxYaw{179.f};
    glm::vec3 m_WorldUp{0.f, 1.f, 0.f};

    void UpdateSpeed();
    void UpdatePosition(float deltaTime);
    void UpdateRotation();
};
