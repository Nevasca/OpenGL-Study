#include "Inspectors/Components/PilotComponentInspector.h"

#include "core/Basics/Components/PilotComponent.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void PilotComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<PilotComponent> pilotComponent = std::dynamic_pointer_cast<PilotComponent>(component);

            assert(pilotComponent);

            float speed = pilotComponent->GetBaseSpeed();
            ImGui::DragFloat("Speed", &speed);

            float fastMultiplier = pilotComponent->GetFastSpeedMultiplier();
            ImGui::DragFloat("Fast Speed Multiplier", &fastMultiplier);

            float sensitivity = pilotComponent->GetBaseSensitivity();
            ImGui::DragFloat("Sensitivity", &sensitivity);

            pilotComponent->SetBaseSpeed(speed);
            pilotComponent->SetFastSpeedMultiplier(fastMultiplier);
            pilotComponent->SetBaseSensitivity(sensitivity);
        }

        int PilotComponentInspector::GetComponentHash()
        {
            return PilotComponent::GetClassHash();
        }
    }
}
