#include "Basics/Objects/SpotLight.h"

namespace Glacirer
{
    void SpotLight::Initialize()
    {
        GameObject::Initialize();

        m_LightComponent = AddComponent<SpotLightComponent>();
    }

    void SpotLight::SetColor(const glm::vec3& color)
    {
        std::shared_ptr<SpotLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetColor(color);
        }
    }

    void SpotLight::SetRange(float range)
    {
        std::shared_ptr<SpotLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetRange(range);
        }
    }

    void SpotLight::SetInnerCutoffDegrees(float innerCutoffDegrees)
    {
        std::shared_ptr<SpotLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetInnerCutoffDegrees(innerCutoffDegrees);
        }
    }

    void SpotLight::SetOuterCutoffDegrees(float outerCutoffDegrees)
    {
        std::shared_ptr<SpotLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetOuterCutoffDegrees(outerCutoffDegrees);
        }
    }

    void SpotLight::SetCastShadowEnabled(const bool enable)
    {
        std::shared_ptr<SpotLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetCastShadowEnabled(enable);
        }
    }
}
