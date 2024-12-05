#include "DirectionalLightComponentInspector.h"

#include <cassert>

#include "core/Basics/Components/DirectionalLightComponent.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void DirectionalLightComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<DirectionalLightComponent> directionalLight = std::dynamic_pointer_cast<DirectionalLightComponent>(component);

            assert(directionalLight);

            glm::vec3 color = directionalLight->GetColor();
            ImGui::ColorEdit3("Color", &color.r);

            float intensity = directionalLight->GetIntensity();
            ImGui::DragFloat("Intensity", &intensity, 1.f, 0.f);

            float bias = directionalLight->GetBias();
            ImGui::DragFloat("Bias", &bias, 0.001f);

            float normalBias = directionalLight->GetNormalBias();
            ImGui::DragFloat("Normal Bias", &normalBias, 0.001f);

            bool bCastShadow = directionalLight->IsCastShadowEnabled();
            ImGui::Checkbox("Cast Shadow", &bCastShadow);
            
            directionalLight->SetColor(color);
            directionalLight->SetIntensity(intensity);
            directionalLight->SetBias(bias);
            directionalLight->SetNormalBias(normalBias);
            directionalLight->SetCastShadowEnabled(bCastShadow);
        }

        int DirectionalLightComponentInspector::GetComponentHash()
        {
            return DirectionalLightComponent::GetClassHash();
        }
    }
}
