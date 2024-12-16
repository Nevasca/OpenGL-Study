#include "Basics/Objects/DirectionalLight.h"

namespace Glacirer
{
    void DirectionalLight::Initialize()
    {
        GameObject::Initialize();

        m_LightComponent = AddComponent<DirectionalLightComponent>();
    }

    void DirectionalLight::Destroy()
    {
        GameObject::Destroy();

        m_LightComponent.reset();
    }

    void DirectionalLight::SetColor(const glm::vec3& color)
    {
        std::shared_ptr<DirectionalLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetColor(color);
        }
    }

    void DirectionalLight::SetIntensity(const float intensity)
    {
        std::shared_ptr<DirectionalLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetIntensity(intensity);
        }
    }

    void DirectionalLight::SetCastShadowEnabled(const bool enable)
    {
        std::shared_ptr<DirectionalLightComponent> lightComponent = m_LightComponent.lock();

        if(lightComponent)
        {
            lightComponent->SetCastShadowEnabled(enable);
        }
    }
}
