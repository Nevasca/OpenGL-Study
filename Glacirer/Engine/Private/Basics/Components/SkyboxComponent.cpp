#include "Basics/Components/SkyboxComponent.h"

#include "World.h"
#include "GameObject/GameObject.h"
#include "Rendering/Primitive.h"

namespace Glacirer
{
    void SkyboxComponent::Initialize()
    {
        Component::Initialize();

        m_Mesh = Rendering::Primitive::CreateSkyCube();
    }

    void SkyboxComponent::Enable()
    {
        Component::Enable();

        if(IsReadyToDraw())
        {
            AddToWorld();
        }
    }

    void SkyboxComponent::Disable()
    {
        Component::Disable();

        if(IsReadyToDraw())
        {
            RemoveFromWorld();
        }
    }

    void SkyboxComponent::SetCubemap(const std::shared_ptr<Rendering::Cubemap>& skyboxCubemap)
    {
        m_Cubemap = skyboxCubemap;

        if(IsReadyToDraw())
        {
            // We add to world even if already added, so render system can update the used skybox cubemap
            AddToWorld();
        }
    }

    void SkyboxComponent::SetMaterial(const std::shared_ptr<Rendering::Material>& skyboxMaterial)
    {
        m_Material = skyboxMaterial;

        if(IsReadyToDraw())
        {
            AddToWorld();
        }
    }

    void SkyboxComponent::AddToWorld()
    {
        World& world = GetWorld();
        world.SetSkyboxComponent(GetThis());
    }

    void SkyboxComponent::RemoveFromWorld()
    {
        World& world = GetWorld();
        world.RemoveSkyboxComponent(GetThis());
    }
}
