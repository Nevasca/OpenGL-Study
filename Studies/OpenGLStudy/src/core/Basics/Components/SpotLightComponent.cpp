#include "SpotLightComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

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

glm::mat4 SpotLightComponent::GetViewMatrix() const
{
    glm::vec3 lightPosition = GetOwnerPosition();
    return glm::lookAt(lightPosition, lightPosition + GetOwnerTransform().GetForwardVector(), glm::vec3{0.f, 1.f, 0.f});
}

glm::mat4 SpotLightComponent::GetProjectionMatrix(const Rendering::Resolution& shadowResolution) const
{
    float constexpr near = 1.f;
    float constexpr far = 25.f;
    float constexpr fov = 90.f;
    
    float aspect = static_cast<float>(shadowResolution.Width)/static_cast<float>(shadowResolution.Height);

    return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::mat4 SpotLightComponent::GetViewProjectionMatrix(const Rendering::Resolution& shadowResolution) const
{
    return GetProjectionMatrix(shadowResolution) * GetViewMatrix();
}