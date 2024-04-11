#include "CameraComponentInspector.h"

#include "core/Basics/Components/CameraComponent.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void CameraComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<CameraComponent> camera = std::dynamic_pointer_cast<CameraComponent>(component);

            assert(camera);

            float fov = camera->GetFov();
            ImGui::DragFloat("Fov", &fov);

            float nearPlane = camera->GetNearPlane();
            ImGui::DragFloat("Near Plane", &nearPlane);

            float farPlane = camera->GetFarPlane();
            ImGui::DragFloat("Far Plane", &farPlane);

            camera->SetFov(fov);
            camera->SetNearPlane(nearPlane);
            camera->SetFarPlane(farPlane);
        }

        int CameraComponentInspector::GetComponentHash()
        {
            return CameraComponent::GetClassHash();
        }
    }
}
