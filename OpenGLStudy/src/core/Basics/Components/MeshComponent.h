#pragma once

#include "core/GameObject/Component.h"

class Material;
class Mesh;

class MeshComponent : public Component
{
    GENERATE_COMPONENT_BODY(MeshComponent)

public:

    void SetMesh(const std::shared_ptr<Mesh>& mesh);
    void SetMaterial(const std::shared_ptr<Material>& material);
    
    bool IsReadyToDraw() const { return m_Mesh != nullptr && m_Material != nullptr; }
    const std::shared_ptr<Mesh>& GetMesh() const { return m_Mesh; }
    const std::shared_ptr<Material>& GetMaterial() const { return m_Material; }

private:

    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Material> m_Material;
    bool bIsAddedToWorld{false};

    void AddToWorld();
    void RemoveFromWorld();
};
