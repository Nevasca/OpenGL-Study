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
        for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
        {
            gameObject->Update(deltaTime);
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

    void World::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
    {
        m_RenderSystem->AddPointLight(pointLightComponent);
    }

    void World::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
    {
        m_RenderSystem->AddSpotLight(spotLightComponent);
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
