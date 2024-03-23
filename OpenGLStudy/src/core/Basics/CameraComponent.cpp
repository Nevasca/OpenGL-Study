#include "CameraComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"
#include <glm/ext/matrix_transform.hpp>

#include <glm/ext/matrix_clip_space.hpp>

#include "core/Screen.h"

void CameraComponent::Initialize()
{
    Component::Initialize();

    World& world = GetOwner().GetWorld();
    world.SetActiveCamera(GetThis());
}

glm::mat4 CameraComponent::GetViewMatrix() const
{
    const Transform& OwnerTransform = GetOwner().GetTransform();
    return glm::lookAt(OwnerTransform.GetPosition(), OwnerTransform.GetPosition() + OwnerTransform.GetForwardVector(), m_Up);
}

glm::mat4 CameraComponent::GetProjectionMatrix() const
{
    const float aspect = static_cast<float>(Screen::GetWidth()) / static_cast<float>(Screen::GetHeight());
    return glm::perspective(glm::radians(m_Fov), aspect, m_NearPlane, m_FarPlane);
}

void CameraComponent::SetFov(float fov)
{
    m_Fov = fov;
}
