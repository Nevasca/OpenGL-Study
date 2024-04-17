#include "Sphere.h"

#include "core/ResourceManager.h"
#include "core/Basics/Components/MeshComponent.h"

void Sphere::Initialize()
{
    GameObject::Initialize();

    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMaterial(ResourceManager::GetDefaultMaterial());
    m_MeshComponent->SetMesh(ResourceManager::GetDefaultSphere());
}

void Sphere::Destroy()
{
    GameObject::Destroy();
    m_MeshComponent.reset();
}

void Sphere::SetMaterial(const std::shared_ptr<Material>& material) const
{
    m_MeshComponent->SetMaterial(material);
}
