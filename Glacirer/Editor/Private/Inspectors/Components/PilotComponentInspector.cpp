#include "Inspectors/Components/PilotComponentInspector.h"

#include "Basics/Components/PilotComponent.h"
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    namespace Inspector
    {
        void PilotComponentInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            std::shared_ptr<Glacirer::PilotComponent> pilotComponent = std::dynamic_pointer_cast<Glacirer::PilotComponent>(component);

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
            return Glacirer::PilotComponent::GetClassHash();
        }
    }
}
