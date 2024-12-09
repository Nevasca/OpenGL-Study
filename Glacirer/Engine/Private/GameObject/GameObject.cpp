#include "GameObject/GameObject.h"

#include "World.h"

namespace Glacirer
{
    GameObject::GameObject(World& world)
        : m_World(world)
    { }

    void GameObject::Initialize()
    {
        m_Id = m_World.GenerateUniqueId();
    }

    void GameObject::Start()
    { }

    void GameObject::Update(float deltaTime)
    {
        for(const std::shared_ptr<Component>& component : m_Components)
        {
            if(component->IsEnabled())
            {
                component->Update(deltaTime);
            }
        }
    }

    void GameObject::Destroy()
    {
        for(const std::shared_ptr<Component>& component : m_Components)
        {
            component->Disable();
            component->Destroy();
        }

        m_Components.clear();
        b_IsPendingDestroy = true;
    }

    void GameObject::SetPosition(const glm::vec3& position)
    {
        m_Transform.SetPosition(position);
    }

    void GameObject::SetRotation(const glm::vec3& eulerRotation)
    {
        m_Transform.SetRotation(eulerRotation);
    }

    void GameObject::SetScale(const glm::vec3& scale)
    {
        m_Transform.SetScale(scale);
    }
}
