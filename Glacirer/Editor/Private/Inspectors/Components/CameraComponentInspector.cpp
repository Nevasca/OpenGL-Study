#include "Inspectors/Components/CameraComponentInspector.h"

#include "Basics/Components/CameraComponent.h"
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    namespace Inspector
    {
        void CameraComponentInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            std::shared_ptr<Glacirer::CameraComponent> camera = std::dynamic_pointer_cast<Glacirer::CameraComponent>(component);

            assert(camera);

            float nearPlane = camera->GetNearPlane();
            ImGui::DragFloat("Near Plane", &nearPlane);

            float farPlane = camera->GetFarPlane();
            ImGui::DragFloat("Far Plane", &farPlane);
            
            camera->SetNearPlane(nearPlane);
            camera->SetFarPlane(farPlane);

            bool bIsOrthographic = camera->IsOrthographic();
            ImGui::Checkbox("Orthographic", &bIsOrthographic);
            camera->SetIsOrthographic(bIsOrthographic);

            if(bIsOrthographic)
            {
                RenderOrthographicGUI(*camera);
            }
            else
            {
                RenderPerspectiveGUI(*camera);
            }
        }

        void CameraComponentInspector::RenderOrthographicGUI(Glacirer::CameraComponent& camera)
        {
            bool bEnableAspectCorrection = camera.IsOrthographicAspectCorrectionEnabled();
            ImGui::Checkbox("Aspect Correction", &bEnableAspectCorrection);

            float width = camera.GetOrthographicWidth();
            float height = camera.GetOrthographicHeight();

            ImGui::DragFloat("Width", &width);

            if(!bEnableAspectCorrection)
            {
                ImGui::DragFloat("Height", &height);
            }

            camera.SetOrthographicSize(width, height, bEnableAspectCorrection);
        }

        void CameraComponentInspector::RenderPerspectiveGUI(Glacirer::CameraComponent& camera)
        {
            float fov = camera.GetFov();
            ImGui::DragFloat("Fov", &fov);

            camera.SetFov(fov);
        }

        int CameraComponentInspector::GetComponentHash()
        {
            return Glacirer::CameraComponent::GetClassHash();
        }
    }
}
