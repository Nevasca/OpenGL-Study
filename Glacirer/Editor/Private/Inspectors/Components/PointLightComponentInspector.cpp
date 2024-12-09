#include "Inspectors/Components/PointLightComponentInspector.h"
#include "Basics/Components/PointLightComponent.h"

#include <cassert>
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    namespace Inspector
    {
        void PointLightComponentInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            std::shared_ptr<Glacirer::PointLightComponent> pointLight = std::dynamic_pointer_cast<Glacirer::PointLightComponent>(component);

            assert(pointLight);

            glm::vec3 color = pointLight->GetColor();
            ImGui::ColorEdit3("Color", &color.r);

            float intensity = pointLight->GetIntensity();
            ImGui::DragFloat("Intensity", &intensity);

            float range = pointLight->GetRange();
            ImGui::DragFloat("Range", &range);

            bool bCastShadow = pointLight->IsCastShadowEnabled();
            ImGui::Checkbox("Cast Shadow", &bCastShadow);

            pointLight->SetColor(color);
            pointLight->SetIntensity(intensity);
            pointLight->SetRange(range);
            pointLight->SetCastShadowEnabled(bCastShadow);
        }

        int PointLightComponentInspector::GetComponentHash()
        {
            return Glacirer::PointLightComponent::GetClassHash();
        }
    }
}
