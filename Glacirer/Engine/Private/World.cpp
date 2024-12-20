#include "World.h"

#include "GameObject/GameObject.h"
#include "Basics/Components/MeshComponent.h"
#include "Basics/Components/CameraComponent.h"

namespace Glacirer
{
    void World::Initialize(const std::shared_ptr<Rendering::RenderSystem>& renderSystem)
    {
        m_RenderSystem = renderSystem;
    }

    void World::Setup()
    { }

    void World::Shutdown()
    {
        for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
        {
            gameObject->Destroy();
        }

        m_RenderSystem.reset();
        m_GameObjects.clear();
    }

    void World::Update(float deltaTime)
    {
        UpdateGameObjects(deltaTime);
        DestroyPendingGameObjects();
    }

    void World::UpdateGameObjects(float deltaTime)
    {
        for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
        {
            gameObject->Update(deltaTime);
        }
    }

    void World::DestroyPendingGameObjects()
    {
        auto gameObjectIterator = m_GameObjects.begin();
        while (gameObjectIterator != m_GameObjects.end())
        {
            if (gameObjectIterator->get()->IsPendingDestroy())
            {
                gameObjectIterator = m_GameObjects.erase(gameObjectIterator);
            }
            else
            {
                ++gameObjectIterator;
            }
        }
    }

    void World::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
    {
        m_RenderSystem->AddMeshComponent(meshComponent);
    }

    void World::RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
    {
        m_RenderSystem->RemoveMeshComponent(meshComponent);
    }

    void World::RemoveMeshComponentsUsing(const std::shared_ptr<Rendering::Material>& material)
    {
        m_RenderSystem->RemoveMeshComponentsUsing(material);
    }

    void World::AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
    {
        m_RenderSystem->AddOutlinedMeshComponent(meshComponent);
    }

    void World::RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
    {
        m_RenderSystem->RemoveOutlinedMeshComponent(meshComponent);
    }

    void World::SetActiveCamera(const std::shared_ptr<CameraComponent>& camera)
    {
        m_ActiveCamera = camera;
    }

    void World::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
    {
        m_RenderSystem->AddDirectionalLight(directionalLightComponent);
    }

    void World::RemoveDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
    {
        m_RenderSystem->RemoveDirectionalLight(directionalLightComponent);
    }

    void World::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
    {
        m_RenderSystem->AddPointLight(pointLightComponent);
    }

    void World::RemovePointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
    {
        m_RenderSystem->RemovePointLight(pointLightComponent);
    }

    void World::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
    {
        m_RenderSystem->AddSpotLight(spotLightComponent);
    }

    void World::RemoveSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
    {
        m_RenderSystem->RemoveSpotLight(spotLightComponent);
    }

    void World::SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
    {
        m_RenderSystem->SetPostProcessingComponent(postProcessingComponent);
    }

    void World::RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
    {
        m_RenderSystem->RemovePostProcessingComponent(postProcessingComponent);
    }

    void World::SetSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent)
    {
        m_RenderSystem->SetSkyboxComponent(skyboxComponent);
    }

    void World::RemoveSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent)
    {
        m_RenderSystem->RemoveSkyboxComponent(skyboxComponent);
    }

    void World::InitializeGameObject(const std::shared_ptr<GameObject>& gameObject) const
    {
        gameObject->Initialize();
        gameObject->SetName("GameObject" + std::to_string(gameObject->GetId()));
        gameObject->Start();
    }

    void World::InitializeGameObject(const std::shared_ptr<GameObject>& gameObject, const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale) const
    {
        gameObject->SetPosition(position);
        gameObject->SetRotation(eulerRotation);
        gameObject->SetScale(scale);

        gameObject->Initialize();
        gameObject->SetName("GameObject" + std::to_string(gameObject->GetId()));
        gameObject->Start();
    }
}
