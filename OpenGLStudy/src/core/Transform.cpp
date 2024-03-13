#include "Transform.h"

#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

void Transform::SetPosition(glm::vec3 position)
{
    m_Position = position;
}

void Transform::SetRotation(glm::vec3 eulerRotation)
{
    m_Rotation = eulerRotation;
}

void Transform::SetScale(glm::vec3 scale)
{
    m_Scale = scale;
}

glm::vec3 Transform::GetPosition() const
{
    return m_Position;
}

glm::vec3 Transform::GetRotation() const
{
    return m_Rotation;
}

glm::vec3 Transform::GetScale() const
{
    return m_Scale;
}

glm::vec3 Transform::GetForwardVector() const
{
    glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);
    glm::vec3 worldForward{0.f, 0.f, 1.f};
    glm::vec3 forward = glm::normalize(rotationMatrix * worldForward);

    return forward;
}

glm::mat4 Transform::GetMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, m_Position);
    glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
    modelMatrix *= rotationMatrix;
    modelMatrix = glm::scale(modelMatrix, m_Scale);

    return modelMatrix;
}
