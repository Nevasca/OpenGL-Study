#include "PilotCameraControllerInspector.h"

#include "core/Basics/Components/PilotCameraController.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void PilotCameraControllerInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            ImGui::TextWrapped("F: enter pilot Mode");
            ImGui::TextWrapped("Esc: exit pilot mode");
        }

        int PilotCameraControllerInspector::GetComponentHash()
        {
            return PilotCameraController::GetClassHash();
        }
    }
}
