#include "PointLightComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"

void PointLightComponent::Initialize()
{
    Component::Initialize();

    SetRange(m_Range);

    World& world = GetOwner().GetWorld();
    world.AddPointLight(GetThis());
}

void PointLightComponent::SetAttenuation(float constant, float linear, float quadratic)
{
    m_Attenuation.Constant = constant;
    m_Attenuation.Linear = linear;
    m_Attenuation.Quadratic = quadratic;
}

glm::vec3 PointLightComponent::GetPosition() const
{
    return GetOwnerPosition();
}

void PointLightComponent::SetRange(float range)
{
    m_Range = std::max(range, 0.001f);
    
    // Linear and Quadratic values are based on this table:
    // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // 4.5f and 75.f values get close to those values when formula is based on desired range
    m_Attenuation.Linear = 4.5f / m_Range;
    m_Attenuation.Quadratic = 75.f / (m_Range * m_Range);
    m_Attenuation.Constant = 1.f;
}
