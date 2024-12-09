#pragma once
#include "GameObject/GameObject.h"

namespace Glacirer
{
    class MeshComponent;

    namespace Rendering
    {
        class Material;
    }

    class ENGINE_API Quad : public GameObject
    {
        GENERATE_OBJECT_BODY(Quad)

    public:

        void Initialize() override;
        void Destroy() override;

        void SetMaterial(const std::shared_ptr<Rendering::Material>& material) const;

    private:

        std::shared_ptr<MeshComponent> m_MeshComponent{};
    };
}
