#pragma once
#include "core/GameObject/GameObject.h"

#include "core/Basics/Components/PointLightComponent.h"

class PointLight : public GameObject
{
    GENERATE_OBJECT_BODY(PointLight)
    
public:

    void Initialize() override;

    void SetColor(const glm::vec3& color) { m_LightComponent->SetColor(color); }
    void SetRange(float range) { m_LightComponent->SetRange(range); }
    void SetIntensity(const float intensity) { m_LightComponent->SetIntensity(intensity); }
    void SetCastShadowEnabled(const bool enable) { m_LightComponent->SetCastShadowEnabled(enable); }

private:

    std::shared_ptr<PointLightComponent> m_LightComponent{};
};
