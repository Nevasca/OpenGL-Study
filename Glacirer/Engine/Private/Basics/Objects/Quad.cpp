#include "Basics/Objects/Quad.h"

#include "Resources/ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

namespace Glacirer
{
    void Quad::Initialize()
    {
        GameObject::Initialize();

        m_MeshComponent = AddComponent<MeshComponent>(); 
        std::shared_ptr<MeshComponent> meshComponent = m_MeshComponent.lock();
        assert(meshComponent);
        
        meshComponent->SetMesh(Resources::ResourceManager::GetDefaultQuad());
        meshComponent->SetMaterial(Resources::ResourceManager::GetDefaultMaterial());
    }

    void Quad::Destroy()
    {
        GameObject::Destroy();

        m_MeshComponent.reset();
    }

    void Quad::SetMaterial(const std::shared_ptr<Rendering::Material>& material) const
    {
        std::shared_ptr<MeshComponent> meshComponent = m_MeshComponent.lock();

        if(meshComponent)
        {
            meshComponent->SetMaterial(material);
        }
    }
}
