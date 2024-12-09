#include "Basics/Components/DirectionalLightComponent.h"

#include "World.h"
#include "GameObject/GameObject.h"
#include "GameObject/Transform.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Glacirer
{
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
        // TODO: implement better ortho size calculation and positioning to improve shadow quality
        constexpr float width = 50.f;
        return glm::ortho(-width / 2.f, width / 2.f, -width /2.f, width / 2.f, 1.f, 30.f);
    }

    glm::mat4 DirectionalLightComponent::GetViewProjectionMatrix() const
    {
        return GetProjectionMatrix() * GetViewMatrix();
    }
}
