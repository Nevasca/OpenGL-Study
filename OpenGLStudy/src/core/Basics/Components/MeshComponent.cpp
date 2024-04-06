#include "MeshComponent.h"

#include "core/GameObject/GameObject.h"
#include "core/World.h"

void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
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

void MeshComponent::SetMaterial(const std::shared_ptr<Material>& material)
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

void MeshComponent::AddToWorld()
{
    if(bIsAddedToWorld)
    {
        return;
    }

    World& world = GetOwner().GetWorld();
    world.AddMeshComponent(GetThis());

    bIsAddedToWorld = true;
}

void MeshComponent::RemoveFromWorld()
{
    World& world = GetOwner().GetWorld();
    world.RemoveMeshComponent(GetThis());

    bIsAddedToWorld = false;
}
