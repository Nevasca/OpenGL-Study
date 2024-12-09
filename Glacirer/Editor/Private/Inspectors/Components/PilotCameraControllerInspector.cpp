#include "Inspectors/Components/PilotCameraControllerInspector.h"

#include "Basics/Components/PilotCameraController.h"
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    namespace Inspector
    {
        void PilotCameraControllerInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            ImGui::TextWrapped("F: enter pilot Mode");
            ImGui::TextWrapped("Esc: exit pilot mode");
        }

        int PilotCameraControllerInspector::GetComponentHash()
        {
            return Glacirer::PilotCameraController::GetClassHash();
        }
    }
}
