#pragma once
#include "core/GameObject/Component.h"
#include "core/Rendering/Light.h"

class SpotLightComponent : public Component
{
    GENERATE_COMPONENT_BODY(SpotLightComponent)

public:

    void Initialize() override;
    glm::vec3 GetDirection() const;
    glm::vec3 GetPosition() const;
    void SetRange(float range);

    float GetRange() const { return m_Range; }
    void SetColor(const glm::vec3& color) { m_Color = color; }
    glm::vec3 GetColor() const { return m_Color; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    float GetIntensity() const { return m_Intensity; }
    void SetInnerCutoffDegrees(float innerCutoffDegrees) { m_InnerCutoffDegrees = innerCutoffDegrees; }
    float GetInnerCutoffDegrees() const { return m_InnerCutoffDegrees; }
    float GetOuterCutoffDegrees() const { return m_OuterCutoffDegrees; }
    void SetOuterCutoffDegrees(float outerCutoffDegrees) { m_OuterCutoffDegrees = outerCutoffDegrees; }
    Attenuation GetAttenuation() const { return m_Attenuation; }

private:

    glm::vec3 m_Color{1.f};
    float m_Intensity{1.f};
    float m_InnerCutoffDegrees{12.f};
    float m_OuterCutoffDegrees{17.5f};
    float m_Range{32.f};
    Attenuation m_Attenuation{};
};
