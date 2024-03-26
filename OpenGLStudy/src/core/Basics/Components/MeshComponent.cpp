#include "MeshComponent.h"

#include "core/GameObject/GameObject.h"
#include "core/World.h"

void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_Mesh = mesh;

    if(IsReadyToDraw())
    {
        AddToWorld();
    }
}

void MeshComponent::SetShader(const std::shared_ptr<Shader>& shader)
{
    m_Shader = shader;

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
