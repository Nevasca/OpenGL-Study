#pragma once
#include "core/GameObject/Component.h"
#include "glm/vec4.hpp"

class DirectionalLightComponent : public Component
{
    GENERATE_COMPONENT_BODY(DirectionalLightComponent)
    
public:

    void Initialize() override;
    glm::vec3 GetDirection() const;

    void SetColor(const glm::vec3& color) { m_Color = color; }
    glm::vec3 GetColor() const { return m_Color; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    float GetIntensity() const { return m_Intensity; }

private:

    glm::vec3 m_Color{1.f};
    float m_Intensity{1.f};
};
