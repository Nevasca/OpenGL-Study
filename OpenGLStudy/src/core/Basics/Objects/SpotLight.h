#pragma once
#include "core/Basics/Components/SpotLightComponent.h"
#include "core/GameObject/GameObject.h"

class SpotLight : public GameObject
{
    GENERATE_OBJECT_BODY(SpotLight)
    
public:

    void Initialize() override;

    void SetColor(const glm::vec3& color) { m_Light->SetColor(color); }
    void SetRange(float range) { m_Light->SetRange(range); }
    void SetInnerCutoffDegrees(float innerCutoffDegrees) { m_Light->SetInnerCutoffDegrees(innerCutoffDegrees); }
    void SetOuterCutoffDegrees(float outerCutoffDegrees) { m_Light->SetOuterCutoffDegrees(outerCutoffDegrees); }

private:

    std::shared_ptr<SpotLightComponent> m_Light;
};
