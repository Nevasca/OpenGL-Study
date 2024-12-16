#include "Basics/Objects/Sphere.h"

#include "Resources/ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

namespace Glacirer
{
    void Sphere::Initialize()
    {
        GameObject::Initialize();

        m_MeshComponent = AddComponent<MeshComponent>();
        std::shared_ptr<MeshComponent> meshComponent = m_MeshComponent.lock();
        assert(meshComponent);

        meshComponent->SetMaterial(Resources::ResourceManager::GetDefaultMaterial());
        meshComponent->SetMesh(Resources::ResourceManager::GetDefaultSphere());
    }

    void Sphere::Destroy()
    {
        GameObject::Destroy();
        m_MeshComponent.reset();
    }

    void Sphere::SetMaterial(const std::shared_ptr<Rendering::Material>& material) const
    {
        std::shared_ptr<MeshComponent> meshComponent = m_MeshComponent.lock();

        if(meshComponent)
        {
            meshComponent->SetMaterial(material);
        }
    }
}
