#include "Component.h"

#include "GameObject.h"

Component::Component(GameObject& owner)
    : m_Owner(owner)
{ }

void Component::Initialize()
{ }

void Component::Start()
{ }

void Component::Update(float deltaTime)
{ }

void Component::Destroy()
{ }

void Component::SetOwnerPosition(const glm::vec3& position) const
{
    m_Owner.SetPosition(position);
}

void Component::SetOwnerRotation(const glm::vec3& eulerRotation) const
{
    m_Owner.SetRotation(eulerRotation);
}

void Component::SetOwnerScale(const glm::vec3& scale) const
{
    m_Owner.SetScale(scale);
}

glm::vec3 Component::GetOwnerPosition() const
{
    return m_Owner.GetPosition();
}

glm::vec3 Component::GetOwnerRotation() const
{
    return m_Owner.GetRotation();
}

glm::vec3 Component::GetOwnerScale() const
{
    return m_Owner.GetScale();
}
