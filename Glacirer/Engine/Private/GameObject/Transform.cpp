#include "GameObject/Transform.h"

#include <glm/gtc/quaternion.hpp>

Transform::Transform(const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale)
    : m_Position(position), m_Rotation(eulerRotation), m_Scale(scale)
{ }

void Transform::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    bIsDirty = true;
}

void Transform::SetRotation(const glm::vec3& eulerRotation)
{
    m_Rotation = eulerRotation;
    bIsDirty = true;
}

void Transform::SetScale(const glm::vec3& scale)
{
    m_Scale = scale;
    bIsDirty = true;
}

void Transform::SetPositionRotationScale(const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale)
{
    m_Position = position;
    m_Rotation = eulerRotation;
    m_Scale = scale;

    bIsDirty = true;
}

glm::vec3 Transform::GetForwardVector() const
{
    glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);
    glm::vec3 worldForward{0.f, 0.f, -1.f};
    glm::vec3 forward = glm::normalize(rotationMatrix * worldForward);

    return forward;
}

glm::vec3 Transform::GetRightVector() const
{
    return glm::normalize(glm::cross(GetForwardVector(), GetUpVector()));
}

glm::vec3 Transform::GetUpVector() const
{
    glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);
    glm::vec3 worldUp{0.f, 1.f, 0.f};
    glm::vec3 up = glm::normalize(rotationMatrix * worldUp);

    return up;
}

glm::mat4 Transform::GetMatrix() const
{
    if(!bIsDirty)
    {
        return m_CachedMatrix;
    }

    m_CachedMatrix = glm::mat4(1.f);
    m_CachedMatrix = glm::translate(m_CachedMatrix, m_Position);
    glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
    m_CachedMatrix *= rotationMatrix;
    m_CachedMatrix = glm::scale(m_CachedMatrix, m_Scale);

    bIsDirty = false;

    return m_CachedMatrix;
}
