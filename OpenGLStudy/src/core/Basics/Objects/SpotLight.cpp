#include "SpotLight.h"

void SpotLight::Initialize()
{
    GameObject::Initialize();

    m_Light = AddComponent<SpotLightComponent>();
}
