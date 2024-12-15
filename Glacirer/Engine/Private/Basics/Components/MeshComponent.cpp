#include "Basics/Components/MeshComponent.h"

#include "GameObject/GameObject.h"
#include "World.h"
#include "Rendering/Material.h"

namespace Glacirer
{
    void MeshComponent::SetMesh(const std::shared_ptr<Rendering::Mesh>& mesh)
    {
        if(bIsAddedToWorld)
        {
            RemoveFromWorld();
        }

        m_Mesh = mesh;

        if(IsReadyToDraw())
        {
            AddToWorld();
        }
    }

    void MeshComponent::SetMaterial(const std::shared_ptr<Rendering::Material>& material)
    {
        if(bIsAddedToWorld)
        {
            RemoveFromWorld();
        }

        m_Material = material;

        if(IsReadyToDraw())
        {
            AddToWorld();
        }
    }

    void MeshComponent::SetIsOutlined(const bool bOutlined)
    {
        if(bIsOutlined == bOutlined)
        {
            return;
        }

        if(bIsAddedToWorld)
        {
            RemoveFromWorld();
        }

        bIsOutlined = bOutlined;

        if(IsReadyToDraw())
        {
            AddToWorld();
        }
    }

    std::shared_ptr<Rendering::Shader> MeshComponent::GetShader() const
    {
        return m_Material ? m_Material->GetShader() : nullptr;   
    }

    void MeshComponent::AddToWorld()
    {
        if(bIsAddedToWorld)
        {
            return;
        }

        World& world = GetOwner().GetWorld();

        if(bIsOutlined)
        {
            world.AddOutlinedMeshComponent(GetThis());
        }
        else
        {
            world.AddMeshComponent(GetThis());
        }

        bIsAddedToWorld = true;
    }

    void MeshComponent::RemoveFromWorld()
    {
        World& world = GetOwner().GetWorld();

        if(bIsOutlined)
        {
            world.RemoveOutlinedMeshComponent(GetThis());
        }
        else
        {
            world.RemoveMeshComponent(GetThis());
        }

        bIsAddedToWorld = false;
    }

    void MeshComponent::Destroy()
    {
        Component::Destroy();

        if(bIsAddedToWorld)
        {
            RemoveFromWorld();
        }
    }
}
