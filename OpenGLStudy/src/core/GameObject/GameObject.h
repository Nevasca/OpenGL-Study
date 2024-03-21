#pragma once

#include <memory>
#include <memory>
#include <vector>

#include "Transform.h"
#include "Component.h"

class GameObject
{
public:

    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void Initialize();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Destroy();
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& eulerRotation);
    void SetScale(const glm::vec3& scale);

    glm::vec3 GetPosition() const { return m_Transform.GetPosition(); }
    glm::vec3 GetRotation() const { return m_Transform.GetRotation(); }
    glm::vec3 GetScale() const { return m_Transform.GetScale(); }
    Transform& GetTransform() { return m_Transform; }

    bool IsPendingDestroy() const { return b_IsPendingDestroy; }

    template <typename TComponent, typename = std::enable_if_t<std::is_base_of_v<Component, TComponent>>>
    std::shared_ptr<TComponent> AddComponent()
    {
        std::shared_ptr<TComponent> component = std::make_shared<TComponent>(*this);
        component->Initialize();
        component->Start();

        m_Components.push_back(component);

        return component;
    }

    template <typename TComponent, typename = std::enable_if_t<std::is_base_of_v<Component, TComponent>>>
    std::shared_ptr<TComponent> GetComponent()
    {
        for(const std::shared_ptr<Component>& component : m_Components)
        {
            std::shared_ptr<TComponent> castedComponent = std::dynamic_pointer_cast<TComponent>(component);

            if(castedComponent)
            {
                return castedComponent;
            }
        }

        return nullptr;
    }

protected:

    Transform m_Transform{};
    std::vector<std::shared_ptr<Component>> m_Components;

private:

    bool b_IsPendingDestroy{false};
};
