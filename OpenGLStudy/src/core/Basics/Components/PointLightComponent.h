#pragma once
#include "core/GameObject/Component.h"

class PointLightComponent : public Component
{
    GENERATE_COMPONENT_BODY(PointLightComponent)
    
public:

    struct Attenuation
    {
        float Constant{1.f};
        float Linear{0.14f};
        float Quadratic{0.07f};
    };

    void Initialize() override;

    void SetAttenuation(float constant, float linear, float quadratic);
    glm::vec3 GetPosition() const;

    void SetRange(float range);
    float GetRange() const { return m_Range; }
    void SetAttenuation(const Attenuation& attenuation) { m_Attenuation = attenuation; }
    Attenuation GetAttenuation() const { return m_Attenuation; }
    void SetColor(const glm::vec3& color) { m_Color = color; }
    glm::vec3 GetColor() const { return m_Color; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    float GetIntensity() const { return m_Intensity; }

private:

    glm::vec3 m_Color{1.f};
    Attenuation m_Attenuation{};
    float m_Intensity{1.f};
    float m_Range{32.f};
};
