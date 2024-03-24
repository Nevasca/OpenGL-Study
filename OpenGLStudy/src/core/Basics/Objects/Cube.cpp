#include "Cube.h"

#include <memory>

#include "core/ResourceManager.h"
#include "core/Basics/Components/MeshComponent.h"

void Cube::Initialize()
{
    GameObject::Initialize();

    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMesh(ResourceManager::GetMesh(ResourceManager::DEFAULT_MESH_CUBE_NAME));
    m_MeshComponent->SetShader(ResourceManager::GetShader(ResourceManager::DEFAULT_MATERIAL_NAME));
}

void Cube::Destroy()
{
    GameObject::Destroy();

    m_MeshComponent.reset();
}

void Cube::SetShader(const std::shared_ptr<Shader>& shader) const
{
    m_MeshComponent->SetShader(shader);
}
