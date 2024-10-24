#include "DirectionalLightComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

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

glm::mat4 DirectionalLightComponent::GetViewMatrix() const
{
    // TODO: implement to be position agnostic (maybe use the camera position?)
    glm::vec3 lightPosition = GetOwnerPosition();
    return glm::lookAt(lightPosition, lightPosition + GetOwnerTransform().GetForwardVector(), glm::vec3{0.f, 1.f, 0.f});
}

glm::mat4 DirectionalLightComponent::GetProjectionMatrix() const
{
    return glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 40.f);
}

glm::mat4 DirectionalLightComponent::GetViewProjectionMatrix() const
{
    return GetProjectionMatrix() * GetViewMatrix();
}
