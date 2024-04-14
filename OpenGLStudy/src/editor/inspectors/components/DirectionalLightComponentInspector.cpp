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
            
            directionalLight->SetColor(color);
            directionalLight->SetIntensity(intensity);
        }

        int DirectionalLightComponentInspector::GetComponentHash()
        {
            return DirectionalLightComponent::GetClassHash();
        }
    }
}
