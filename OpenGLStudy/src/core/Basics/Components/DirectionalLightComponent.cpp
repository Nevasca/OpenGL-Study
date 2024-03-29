#include "DirectionalLightComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"

void DirectionalLightComponent::Initialize()
{
    Component::Initialize();

    World& world = GetOwner().GetWorld();
    world.AddDirectionalLight(GetThis());
}

glm::vec3 DirectionalLightComponent::GetDirection() const
{
    return GetOwnerTransform().GetForwardVector();
}
