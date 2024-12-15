#pragma once

#include "GameObject/Component.h"

namespace Glacirer
{
    namespace Rendering
    {
        class Shader;
        class Material;
        class Mesh;
    }

    class ENGINE_API MeshComponent : INHERIT_FROM_COMPONENT(MeshComponent)
    {
        GENERATE_COMPONENT_BODY(MeshComponent)

    public:

        void SetMesh(const std::shared_ptr<Rendering::Mesh>& mesh);
        void SetMaterial(const std::shared_ptr<Rendering::Material>& material);
        void SetIsOutlined(const bool bOutlined);
    
        bool IsReadyToDraw() const { return m_Mesh != nullptr && m_Material != nullptr; }
        const std::shared_ptr<Rendering::Mesh>& GetMesh() const { return m_Mesh; }
        const std::shared_ptr<Rendering::Material>& GetMaterial() const { return m_Material; }
        std::shared_ptr<Rendering::Shader> GetShader() const;
        bool IsOutlined() const { return bIsOutlined; }

    private:

        std::shared_ptr<Rendering::Mesh> m_Mesh;
        std::shared_ptr<Rendering::Material> m_Material;
        bool bIsAddedToWorld{false};
        bool bIsOutlined{false};

        void AddToWorld();
        void RemoveFromWorld();
    };
}
