#pragma once
#include "GameObject/Component.h"

namespace Glacirer
{
    namespace Rendering
    {
        class Cubemap;
        class Material;
        class Mesh;
    }


    class ENGINE_API SkyboxComponent : public Component
    {
        GENERATE_COMPONENT_BODY(SkyboxComponent)

    public:

        void Initialize() override;
        void Enable() override;
        void Disable() override;

        void SetCubemap(const std::shared_ptr<Rendering::Cubemap>& skyboxCubemap);
        void SetMaterial(const std::shared_ptr<Rendering::Material>& skyboxMaterial);
    
        const std::shared_ptr<Rendering::Mesh>& GetMesh() const { return m_Mesh; }
        const std::shared_ptr<Rendering::Material>& GetMaterial() const { return m_Material; }
        const std::shared_ptr<Rendering::Cubemap>& GetCubemap() const { return m_Cubemap; }
        bool IsReadyToDraw() const { return m_Material != nullptr && m_Cubemap != nullptr; }

    private:

        std::shared_ptr<Rendering::Mesh> m_Mesh;
        std::shared_ptr<Rendering::Cubemap> m_Cubemap{};
        std::shared_ptr<Rendering::Material> m_Material;

        void AddToWorld();
        void RemoveFromWorld();
    };
}
