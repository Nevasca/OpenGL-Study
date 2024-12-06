#pragma once
#include "GameObject/GameObject.h"
#include "Basics/Components/DirectionalLightComponent.h"

class DirectionalLightComponent;

class ENGINE_API DirectionalLight : public GameObject
{
    GENERATE_OBJECT_BODY(DirectionalLight)
    
public:

    void Initialize() override;
    void Destroy() override;

    void SetColor(const glm::vec3& color) { m_LightComponent->SetColor(color); }
    void SetIntensity(const float intensity) { m_LightComponent->SetIntensity(intensity); }
    void SetCastShadowEnabled(const bool enable) { m_LightComponent->SetCastShadowEnabled(enable); }

private:

    std::shared_ptr<DirectionalLightComponent> m_LightComponent{};
};
