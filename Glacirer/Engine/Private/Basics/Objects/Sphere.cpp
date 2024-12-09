#include "Basics/Objects/Sphere.h"

#include "Resources/ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

namespace Glacirer
{
    void Sphere::Initialize()
    {
        GameObject::Initialize();

        m_MeshComponent = AddComponent<MeshComponent>();
        m_MeshComponent->SetMaterial(Resources::ResourceManager::GetDefaultMaterial());
        m_MeshComponent->SetMesh(Resources::ResourceManager::GetDefaultSphere());
    }

    void Sphere::Destroy()
    {
        GameObject::Destroy();
        m_MeshComponent.reset();
    }

    void Sphere::SetMaterial(const std::shared_ptr<Rendering::Material>& material) const
    {
        m_MeshComponent->SetMaterial(material);
    }
}
