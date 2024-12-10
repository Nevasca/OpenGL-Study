#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include "Rendering/RenderSystem.h"
#include "EngineAPI.h"

namespace Glacirer
{
    class PostProcessingComponent;
    class SpotLightComponent;
    class PointLightComponent;
    class DirectionalLightComponent;
    class MeshComponent;
    class CameraComponent;
    class GameObject;

    class ENGINE_API World
    {
    public:

        void Initialize(const std::shared_ptr<Rendering::RenderSystem>& renderSystem);
        void Setup();
        void Shutdown();
        void Update(float deltaTime);
        void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
        void RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
        void AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
        void RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
        void SetActiveCamera(const std::shared_ptr<CameraComponent>& camera);
        void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
        void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
        void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
        void SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
        void RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
        void SetSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent);
        void RemoveSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent);

        unsigned int GenerateUniqueId() { return m_LastUsedId++; }
        std::vector<std::shared_ptr<GameObject>>& GetAllGameObjects() { return m_GameObjects; }
        std::shared_ptr<GameObject> GetGameObjectAt(const int index) const { return m_GameObjects[index]; }
        Rendering::RenderSystem& GetRenderSystem() const { return *m_RenderSystem; }
        std::shared_ptr<CameraComponent> GetActiveCamera() const { return m_ActiveCamera; }

        template <typename TObjectType, typename = std::enable_if_t<std::is_base_of_v<GameObject, TObjectType>>>
        std::shared_ptr<TObjectType> Spawn()
        {
            std::shared_ptr<TObjectType> instance = std::make_shared<TObjectType>(*this);
            InitializeGameObject(instance);
    
            m_GameObjects.push_back(instance);
    
            return instance;
        }
    
        template <typename TObjectType, typename = std::enable_if_t<std::is_base_of_v<GameObject, TObjectType>>>
        std::shared_ptr<TObjectType> Spawn(const glm::vec3& position, const glm::vec3& eulerRotation = glm::vec3{0.f}, const glm::vec3& scale = glm::vec3{1.f})
        {
            std::shared_ptr<TObjectType> instance = std::make_shared<TObjectType>(*this);
            InitializeGameObject(instance, position, eulerRotation, scale);
    
            m_GameObjects.push_back(instance);
    
            return instance;
        }

    private:

        void InitializeGameObject(const std::shared_ptr<GameObject>& gameObject) const;
        void InitializeGameObject(const std::shared_ptr<GameObject>& gameObject, const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale) const;
    
        std::vector<std::shared_ptr<GameObject>> m_GameObjects{};
        std::shared_ptr<Rendering::RenderSystem> m_RenderSystem{};
        std::shared_ptr<CameraComponent> m_ActiveCamera{};
        unsigned int m_LastUsedId{0};
    };
}
