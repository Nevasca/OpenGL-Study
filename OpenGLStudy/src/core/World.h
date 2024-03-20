#pragma once
#include <memory>
#include <memory>
#include <vector>

#include "GameObject.h"

class World
{
public:

    void Initialize();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

    template <typename TObjectType, typename = std::enable_if_t<std::is_base_of_v<GameObject, TObjectType>>>
    std::shared_ptr<TObjectType> Spawn()
    {
        std::shared_ptr<TObjectType> instance = std::make_shared<TObjectType>();

        instance->Initialize();
        instance->Start();
    
        m_GameObjects.push_back(instance);
    
        return instance;
    }
    
    template <typename TObjectType, typename = std::enable_if_t<std::is_base_of_v<GameObject, TObjectType>>>
    std::shared_ptr<TObjectType> Spawn(const glm::vec3& position, const glm::vec3& eulerRotation = glm::vec3{0.f}, const glm::vec3 scale = glm::vec3{1.f})
    {
        std::shared_ptr<TObjectType> instance = std::make_shared<TObjectType>();
        instance->SetPosition(position);
        instance->SetRotation(eulerRotation);
        instance->SetScale(scale);

        instance->Initialize();
        instance->Start();
    
        m_GameObjects.push_back(instance);
    
        return instance;
    }

private:

    std::vector<std::shared_ptr<GameObject>> m_GameObjects;
};
