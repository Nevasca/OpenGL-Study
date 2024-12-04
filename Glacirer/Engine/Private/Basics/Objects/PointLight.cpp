#include "PointLight.h"

void PointLight::Initialize()
{
    GameObject::Initialize();

    m_LightComponent = AddComponent<PointLightComponent>();
}
