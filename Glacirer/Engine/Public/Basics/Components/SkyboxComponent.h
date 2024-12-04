#pragma once
#include "GameObject/Component.h"

namespace Rendering
{
    class Cubemap;
}

class Material;
class Mesh;

class SkyboxComponent : public Component
{
    GENERATE_COMPONENT_BODY(SkyboxComponent)

public:

    void Initialize() override;
    void Enable() override;
    void Disable() override;

    void SetCubemap(const std::shared_ptr<Rendering::Cubemap>& skyboxCubemap);
    void SetMaterial(const std::shared_ptr<Material>& skyboxMaterial);
    
    const std::shared_ptr<Mesh>& GetMesh() const { return m_Mesh; }
    const std::shared_ptr<Material>& GetMaterial() const { return m_Material; }
    const std::shared_ptr<Rendering::Cubemap>& GetCubemap() const { return m_Cubemap; }
    bool IsReadyToDraw() const { return m_Material != nullptr && m_Cubemap != nullptr; }

private:

    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Rendering::Cubemap> m_Cubemap{};
    std::shared_ptr<Material> m_Material;

    void AddToWorld();
    void RemoveFromWorld();
};
