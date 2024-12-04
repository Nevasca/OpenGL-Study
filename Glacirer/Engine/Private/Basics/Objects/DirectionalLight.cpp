#include "Basics/Objects/DirectionalLight.h"


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
