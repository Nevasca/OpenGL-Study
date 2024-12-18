#pragma once
#include "GameObject/GameObject.h"

#include "Basics/Components/PointLightComponent.h"

namespace Glacirer
{
    class ENGINE_API PointLight : public GameObject
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
}
