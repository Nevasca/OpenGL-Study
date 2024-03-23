#include "MeshComponent.h"

#include "core/GameObject/GameObject.h"
#include "core/World.h"

void MeshComponent::Initialize()
{
    Component::Initialize();

    World& world = GetOwner().GetWorld();
    world.AddMeshComponent(GetThis());
}

void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_Mesh = mesh;
}

void MeshComponent::SetShader(const std::shared_ptr<Shader>& shader)
{
    m_Shader = shader;
}
