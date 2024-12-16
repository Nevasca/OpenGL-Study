#pragma once
#include "Basics/Components/SpotLightComponent.h"
#include "GameObject/GameObject.h"

namespace Glacirer
{
    class ENGINE_API SpotLight : public GameObject
    {
        GENERATE_OBJECT_BODY(SpotLight)
    
    public:

        void Initialize() override;

        void SetColor(const glm::vec3& color);
        void SetRange(float range);
        void SetInnerCutoffDegrees(float innerCutoffDegrees);
        void SetOuterCutoffDegrees(float outerCutoffDegrees);
        void SetCastShadowEnabled(const bool enable);

    private:

        std::weak_ptr<SpotLightComponent> m_LightComponent;
    };
}
