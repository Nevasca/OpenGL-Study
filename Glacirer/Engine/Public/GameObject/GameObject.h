#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Transform.h"
#include "Component.h"

#define GENERATE_OBJECT_BODY(ClassName) public:\
    ClassName(World& world)\
    : GameObject(world) { }\


namespace Glacirer
{
    class World;

    class ENGINE_API GameObject
    {
    public:

        GameObject(World& world);
        virtual ~GameObject() = default;

        virtual void Initialize();
        virtual void Start();
        virtual void Update(float deltaTime);
        virtual void Destroy();
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& eulerRotation);
        void SetScale(const glm::vec3& scale);

        std::string GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }
        unsigned int GetId() const { return m_Id; }

        glm::vec3 GetPosition() const { return m_Transform.GetPosition(); }
        glm::vec3 GetRotation() const { return m_Transform.GetRotation(); }
        glm::vec3 GetScale() const { return m_Transform.GetScale(); }
        Transform& GetTransform() { return m_Transform; }

        bool IsPendingDestroy() const { return b_IsPendingDestroy; }
        World& GetWorld() const { return m_World; }
        std::vector<std::shared_ptr<Component>> GetComponents() const { return m_Components; }
        void RemoveComponent(const std::shared_ptr<Component>& component);

        // Only accepts components based on the Component class
        template <typename TComponent, typename = std::enable_if_t<
                      std::is_base_of_v<Component, TComponent> && !std::is_same_v<Component, std::remove_cv_t<TComponent>>>>
        std::weak_ptr<TComponent> AddComponent()
        {
            std::shared_ptr<TComponent> component = std::make_shared<TComponent>(*this);

            component->Initialize();
            component->Enable();
            component->Start();

            m_Components.push_back(component);

            return component;
        }

        template <typename TComponent, typename = std::enable_if_t<
                      std::is_base_of_v<Component, TComponent> && !std::is_same_v<Component, std::remove_cv_t<TComponent>>>>
        std::weak_ptr<TComponent> GetComponent()
        {
            for(const std::shared_ptr<Component>& component : m_Components)
            {
                std::shared_ptr<TComponent> castedComponent = std::dynamic_pointer_cast<TComponent>(component);

                if(castedComponent)
                {
                    return castedComponent;
                }
            }

            return std::weak_ptr<TComponent>{};
        }

    protected:

        Transform m_Transform{};
        std::vector<std::shared_ptr<Component>> m_Components{};

    private:

        bool b_IsPendingDestroy{false};
        World& m_World;
        std::string m_Name{};
        unsigned int m_Id{0};
    };
}
