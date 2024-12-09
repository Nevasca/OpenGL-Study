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

        void SetColor(const glm::vec3& color) { m_LightComponent->SetColor(color); }
        void SetRange(float range) { m_LightComponent->SetRange(range); }
        void SetInnerCutoffDegrees(float innerCutoffDegrees) { m_LightComponent->SetInnerCutoffDegrees(innerCutoffDegrees); }
        void SetOuterCutoffDegrees(float outerCutoffDegrees) { m_LightComponent->SetOuterCutoffDegrees(outerCutoffDegrees); }
        void SetCastShadowEnabled(const bool enable) { m_LightComponent->SetCastShadowEnabled(enable); }

    private:

        std::shared_ptr<SpotLightComponent> m_LightComponent;
    };
}
