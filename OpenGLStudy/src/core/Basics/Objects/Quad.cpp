#include "Quad.h"

#include "core/ResourceManager.h"
#include "core/Basics/Components/MeshComponent.h"

void Quad::Initialize()
{
    GameObject::Initialize();

    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMesh(ResourceManager::GetDefaultQuad());
    m_MeshComponent->SetMaterial(ResourceManager::GetDefaultMaterial());
}

void Quad::Destroy()
{
    GameObject::Destroy();

    m_MeshComponent.reset();
}

void Quad::SetMaterial(const std::shared_ptr<Material>& material) const
{
    m_MeshComponent->SetMaterial(material);
}
