#include "World.h"

#include "GameObject/GameObject.h"
#include "Rendering/MeshRenderer.h"
#include <glm/glm.hpp>

#include "Basics/Components/MeshComponent.h"
#include "Basics/Components/CameraComponent.h"
#include "Rendering/Shader.h"

void World::Initialize()
{
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

    const glm::mat4 view = m_ActiveCamera->GetViewMatrix();
    const glm::mat4 proj = m_ActiveCamera->GetProjectionMatrix();

    for(auto& activeShaderPair : m_UniqueActiveShaders)
    {
        Shader& activeShader = *activeShaderPair.second;
        activeShader.Bind();
        activeShader.SetUniformMat4f("u_Proj", proj);
        activeShader.SetUniformMat4f("u_View", view);
    }

    for(const std::shared_ptr<MeshComponent>& meshComponent : m_MeshComponents)
    {
        assert(meshComponent->IsReadyToDraw());

        m_MeshRenderer.Render(*meshComponent->GetMesh(), meshComponent->GetOwner().GetTransform(), *meshComponent->GetShader());
    }
}

void World::Shutdown()
{
    for(const std::shared_ptr<GameObject>& gameObject : m_GameObjects)
    {
        gameObject->Destroy();
    }

    m_MeshComponents.clear();
    m_GameObjects.clear();
    m_UniqueActiveShaders.clear();
}

void World::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    m_MeshComponents.push_back(meshComponent);

    std::shared_ptr<Shader> meshShader = meshComponent->GetShader();

    if(meshShader && m_UniqueActiveShaders.find(meshShader->GetRendererID()) == m_UniqueActiveShaders.end())
    {
        m_UniqueActiveShaders[meshShader->GetRendererID()] = meshShader;
    }
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
