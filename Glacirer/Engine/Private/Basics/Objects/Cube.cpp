#include "Basics/Objects/Cube.h"

#include <memory>

#include "ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

void Cube::Initialize()
{
    GameObject::Initialize();

    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMesh(ResourceManager::GetDefaultCube());
    m_MeshComponent->SetMaterial(ResourceManager::GetDefaultMaterial());
}

void Cube::Destroy()
{
    GameObject::Destroy();

    m_MeshComponent.reset();
}

void Cube::SetMaterial(const std::shared_ptr<Material>& material) const
{
    m_MeshComponent->SetMaterial(material);
}
