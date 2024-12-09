#include "Basics/Objects/Quad.h"

#include "Resources/ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

namespace Glacirer
{
    void Quad::Initialize()
    {
        GameObject::Initialize();

        m_MeshComponent = AddComponent<MeshComponent>();
        m_MeshComponent->SetMesh(Resources::ResourceManager::GetDefaultQuad());
        m_MeshComponent->SetMaterial(Resources::ResourceManager::GetDefaultMaterial());
    }

    void Quad::Destroy()
    {
        GameObject::Destroy();

        m_MeshComponent.reset();
    }

    void Quad::SetMaterial(const std::shared_ptr<Rendering::Material>& material) const
    {
        m_MeshComponent->SetMaterial(material);
    }
}
