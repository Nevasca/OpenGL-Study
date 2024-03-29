#include "SpotLightComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"

void SpotLightComponent::Initialize()
{
    Component::Initialize();

    SetRange(m_Range);
    
    World& World = GetOwner().GetWorld();
    World.AddSpotLight(GetThis());
}

glm::vec3 SpotLightComponent::GetDirection() const
{
    return GetOwnerTransform().GetForwardVector();
}

glm::vec3 SpotLightComponent::GetPosition() const
{
    return GetOwnerPosition();
}

void SpotLightComponent::SetRange(float range)
{
    m_Range = std::max(range, 0.001f);
    m_Attenuation = Light::CalculateAttenuation(range);
}
