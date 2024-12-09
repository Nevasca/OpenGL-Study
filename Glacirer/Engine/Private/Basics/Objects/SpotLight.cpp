#include "Basics/Objects/SpotLight.h"

namespace Glacirer
{
    void SpotLight::Initialize()
    {
        GameObject::Initialize();

        m_LightComponent = AddComponent<SpotLightComponent>();
    }
}
