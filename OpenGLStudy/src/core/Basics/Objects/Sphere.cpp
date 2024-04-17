#include "Sphere.h"

#include "core/ResourceManager.h"
#include "core/Basics/Components/MeshComponent.h"

void Sphere::Initialize()
{
    GameObject::Initialize();

    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMaterial(ResourceManager::GetMaterial(ResourceManager::DEFAULT_MATERIAL_NAME));
    m_MeshComponent->SetMesh(ResourceManager::GetMesh(ResourceManager::DEFAULT_MESH_SPHERE_NAME));
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
