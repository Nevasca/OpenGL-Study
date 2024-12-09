#include "GameObject/Component.h"

#include "GameObject/GameObject.h"
#include "World.h"

namespace Glacirer
{
    Component::Component(GameObject& owner)
        : m_Owner(owner)
    {
        m_Id = m_Owner.GetWorld().GenerateUniqueId();
    }

    void Component::Initialize()
    { }

    void Component::Start()
    { }

    void Component::Update(float deltaTime)
    { }

    void Component::Destroy()
    { }

    void Component::Enable()
    {
        bIsEnabled = true;
    }

    void Component::Disable()
    {
        bIsEnabled = false;
    }

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

    Transform& Component::GetOwnerTransform() const
    {
        return m_Owner.GetTransform();
    }

    World& Component::GetWorld() const
    {
        return m_Owner.GetWorld();
    }
}
