#include "SpotLight.h"

void SpotLight::Initialize()
{
    GameObject::Initialize();

    m_LightComponent = AddComponent<SpotLightComponent>();
}
