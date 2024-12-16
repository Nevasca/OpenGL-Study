#include "Basics/Components/CameraComponent.h"

#include "World.h"
#include "GameObject/GameObject.h"
#include "GameObject/Transform.h"
#include <glm/ext/matrix_transform.hpp>

#include <glm/ext/matrix_clip_space.hpp>

#include "Screen.h"

namespace Glacirer
{
    void CameraComponent::Initialize()
    {
        Component::Initialize();

        SetAsActive();
    }

    void CameraComponent::Destroy()
    {
        Component::Destroy();

        World& world = GetOwner().GetWorld();
        world.SetActiveCamera(nullptr);
    }

    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        const Transform& OwnerTransform = GetOwner().GetTransform();

        return glm::lookAt(OwnerTransform.GetPosition(), OwnerTransform.GetPosition() + OwnerTransform.GetForwardVector(), m_Up);
    }

    glm::mat4 CameraComponent::GetViewNoTranslationMatrix() const
    {
        // We remove translation by converting to a 3x3 matrix and converting it back to a 4x4 matrix
        return glm::mat4(glm::mat3(GetViewMatrix()));
    }

    glm::mat4 CameraComponent::GetViewCenteredMatrix() const
    {
        const Transform& OwnerTransform = GetOwner().GetTransform();

        return glm::lookAt(OwnerTransform.GetPosition(), glm::vec3{0.f}, m_Up);
    }

    glm::mat4 CameraComponent::GetProjectionMatrix() const
    {
        if(!bIsOrthographic)
        {
            return glm::perspective(glm::radians(m_Fov), Screen::GetAspectRatio(), m_NearPlane, m_FarPlane);
        }

        float height = m_OrthographicHeight;

        if(bEnableOrthographicAspectCorrection)
        {
            height = m_OrthographicWidth / Screen::GetAspectRatio();
        }

        return glm::ortho(-m_OrthographicWidth / 2.f, m_OrthographicWidth / 2.f, -height /2.f, height / 2.f, m_NearPlane, m_FarPlane);
    }

    void CameraComponent::SetAsActive()
    {
        World& world = GetOwner().GetWorld();
        world.SetActiveCamera(GetThis());
    }

    void CameraComponent::SetFov(const float fov)
    {
        m_Fov = fov;
    }

    void CameraComponent::SetIsOrthographic(bool bInIsOrthographic)
    {
        bIsOrthographic = bInIsOrthographic;
    }

    void CameraComponent::SetOrthographicSize(const float width, const float height, bool bEnableAspectCorrection)
    {
        m_OrthographicWidth = width;
        m_OrthographicHeight = height;
        bEnableOrthographicAspectCorrection = bEnableAspectCorrection;
    }
}
