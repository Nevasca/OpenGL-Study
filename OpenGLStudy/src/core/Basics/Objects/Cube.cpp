#include "Cube.h"

#include <memory>

#include "core/ResourceManager.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/Rendering/Primitive.h"

void Cube::Initialize()
{
    GameObject::Initialize();

    //TODO: After creating resource manager, get already created cube mesh and default shader
    //instead of creating a new one for every cube instance
    m_MeshComponent = AddComponent<MeshComponent>();
    m_MeshComponent->SetMesh(Primitive::CreateCube());
    m_MeshComponent->SetShader(ResourceManager::GetShader(ResourceManager::DEFAULT_MATERIAL_NAME));
}

void Cube::Destroy()
{
    GameObject::Destroy();

    m_MeshComponent.reset();
}

void Cube::SetShader(const std::shared_ptr<Shader>& shader) const
{
    m_MeshComponent->SetShader(shader);
}
