#include "Basics/Objects/PointLight.h"

namespace Glacirer
{
    void PointLight::Initialize()
    {
        GameObject::Initialize();

        m_LightComponent = AddComponent<PointLightComponent>();
    }
}
