#include "PointLightComponentInspector.h"
#include "core/Basics/Components/PointLightComponent.h"

#include <cassert>
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void PointLightComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<PointLightComponent> pointLight = std::dynamic_pointer_cast<PointLightComponent>(component);

            assert(pointLight);

            glm::vec3 color = pointLight->GetColor();
            ImGui::ColorEdit3("Color", &color.r);

            float intensity = pointLight->GetIntensity();
            ImGui::DragFloat("Intensity", &intensity);

            float range = pointLight->GetRange();
            ImGui::DragFloat("Range", &range);

            pointLight->SetColor(color);
            pointLight->SetIntensity(intensity);
            pointLight->SetRange(range);
        }

        int PointLightComponentInspector::GetComponentHash()
        {
            return PointLightComponent::GetClassHash();
        }
    }
}
