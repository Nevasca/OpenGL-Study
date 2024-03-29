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
    m_Attenuation = Light::CalculateAttenuation(range);
}
