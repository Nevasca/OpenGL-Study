#include "Basics/Objects/PointLight.h"

namespace Glacirer
{
    void PointLight::Initialize()
    {
        GameObject::Initialize();

        m_LightComponent = AddComponent<PointLightComponent>();
    }

    void PointLight::SetColor(const glm::vec3& color)
    {
        std::shared_ptr<PointLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetColor(color);
        }    
    }

    void PointLight::SetRange(float range)
    {
        std::shared_ptr<PointLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetRange(range);
        }  
    }

    void PointLight::SetIntensity(const float intensity)
    {
        std::shared_ptr<PointLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetIntensity(intensity);
        }  
    }

    void PointLight::SetCastShadowEnabled(const bool enable)
    {
        std::shared_ptr<PointLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetCastShadowEnabled(enable);
        }  
    }
}
