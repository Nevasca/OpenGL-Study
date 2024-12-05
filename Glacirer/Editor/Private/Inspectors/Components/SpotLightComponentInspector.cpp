#include "Inspectors/Components/SpotLightComponentInspector.h"

#include "core/Basics/Components/SpotLightComponent.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void SpotLightComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<SpotLightComponent> spotLight = std::dynamic_pointer_cast<SpotLightComponent>(component);

            assert(spotLight);

            glm::vec3 color = spotLight->GetColor();
            ImGui::ColorEdit3("Color", &color.r);

            float intensity = spotLight->GetIntensity();
            ImGui::DragFloat("Intensity", &intensity);

            float range = spotLight->GetRange();
            ImGui::DragFloat("Range", &range);

            float innerCutoff = spotLight->GetInnerCutoffDegrees();
            ImGui::DragFloat("Inner Cutoff Degrees", &innerCutoff);
            
            float outerCutoff = spotLight->GetOuterCutoffDegrees();
            ImGui::DragFloat("Outer Cutoff Degrees", &outerCutoff);

            bool bCastShadow = spotLight->IsCastShadowEnabled();
            ImGui::Checkbox("Cast Shadow", &bCastShadow);
            
            spotLight->SetColor(color);
            spotLight->SetIntensity(intensity);
            spotLight->SetRange(range);
            spotLight->SetInnerCutoffDegrees(innerCutoff);
            spotLight->SetOuterCutoffDegrees(outerCutoff);
            spotLight->SetCastShadowEnabled(bCastShadow);
        }

        int SpotLightComponentInspector::GetComponentHash()
        {
            return SpotLightComponent::GetClassHash();
        }
    }
}
