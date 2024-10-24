#pragma once
#include "core/GameObject/Component.h"
#include "glm/vec4.hpp"
#include <glm/fwd.hpp>

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
    void SetBias(float bias) { m_Bias = bias; }
    float GetBias() const { return m_Bias; }
    void SetNormalBias(float normalBias) { m_NormalBias = normalBias; }
    float GetNormalBias() const { return m_NormalBias; }
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

private:

    glm::vec3 m_Color{1.f};
    float m_Intensity{1.f};
    float m_Bias{0.005f};
    float m_NormalBias{0.001f};
};
