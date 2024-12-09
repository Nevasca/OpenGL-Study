#include "Basics/Objects/Cube.h"

#include <memory>

#include "Resources/ResourceManager.h"
#include "Basics/Components/MeshComponent.h"

namespace Glacirer
{
    void Cube::Initialize()
    {
        GameObject::Initialize();

        m_MeshComponent = AddComponent<MeshComponent>();
        m_MeshComponent->SetMesh(Resources::ResourceManager::GetDefaultCube());
        m_MeshComponent->SetMaterial(Resources::ResourceManager::GetDefaultMaterial());
    }

    void Cube::Destroy()
    {
        GameObject::Destroy();

        m_MeshComponent.reset();
    }

    void Cube::SetMaterial(const std::shared_ptr<Rendering::Material>& material) const
    {
        m_MeshComponent->SetMaterial(material);
    }
}
