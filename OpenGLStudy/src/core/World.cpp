#include "World.h"

#include "GameObject/GameObject.h"
#include "Basics/Components/MeshComponent.h"
#include "Basics/Components/CameraComponent.h"

void World::Initialize()
{
    m_RenderSystem = std::make_unique<RenderSystem>();
}

void World::Update(float deltaTime)
{
    for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

void World::Render()
{
    if(!m_ActiveCamera)
    {
        // TODO: Log warning (create a log class)
        return;        
    }

    m_RenderSystem->Render(*m_ActiveCamera);
}

void World::Shutdown()
{
    for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
    {
        gameObject->Destroy();
    }

    m_RenderSystem->Shutdown();
    m_RenderSystem.reset();
    m_GameObjects.clear();
}

void World::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    m_RenderSystem->AddMeshComponent(meshComponent);
}

void World::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_RenderSystem->AddDirectionalLight(directionalLightComponent);
}

void World::SetActiveCamera(const std::shared_ptr<CameraComponent>& camera)
{
    m_ActiveCamera = camera;
}

void World::InitializeGameObject(const std::shared_ptr<GameObject>& gameObject) const
{
    gameObject->Initialize();
    gameObject->Start();
}

void World::InitializeGameObject(const std::shared_ptr<GameObject>& gameObject, const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale) const
{
    gameObject->SetPosition(position);
    gameObject->SetRotation(eulerRotation);
    gameObject->SetScale(scale);

    gameObject->Initialize();
    gameObject->Start();
}
