#pragma once

#include "GameObject/Component.h"

class Shader;
class Material;
class Mesh;

class ENGINE_API MeshComponent : public Component
{
    GENERATE_COMPONENT_BODY(MeshComponent)

public:

    void SetMesh(const std::shared_ptr<Mesh>& mesh);
    void SetMaterial(const std::shared_ptr<Material>& material);
    void SetIsOutlined(const bool bOutlined);
    
    bool IsReadyToDraw() const { return m_Mesh != nullptr && m_Material != nullptr; }
    const std::shared_ptr<Mesh>& GetMesh() const { return m_Mesh; }
    const std::shared_ptr<Material>& GetMaterial() const { return m_Material; }
    std::shared_ptr<Shader> GetShader() const;
    bool IsOutlined() const { return bIsOutlined; }

private:

    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Material> m_Material;
    bool bIsAddedToWorld{false};
    bool bIsOutlined{false};

    void AddToWorld();
    void RemoveFromWorld();
};
