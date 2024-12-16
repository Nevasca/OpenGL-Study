#include "ComponentAdderPopup.h"

#include "Basics/Components/MeshComponent.h"
#include "GameObject/GameObject.h"
#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/DirectionalLightComponent.h"
#include "Basics/Components/PointLightComponent.h"
#include "Basics/Components/PostProcessingComponent.h"
#include "Basics/Components/SkyboxComponent.h"
#include "Basics/Components/SpotLightComponent.h"

#include <imgui/imgui.h>

namespace GlacirerEditor
{
    constexpr const char* COMPONENT_POPUP_NAME = "ComponentAdderPopup";

    void ComponentAdderPopup::OpenPopup()
    {
        ImGui::OpenPopup(COMPONENT_POPUP_NAME);
    }

    void ComponentAdderPopup::RenderGUI(Glacirer::GameObject& selectedGameObject)
    {
        if(!ImGui::BeginPopup(COMPONENT_POPUP_NAME))
        {
            return;
        }

        ImGui::SeparatorText("Components");
        RenderRenderingSubmenu(selectedGameObject);

        ImGui::EndPopup();
    }

    void ComponentAdderPopup::RenderRenderingSubmenu(Glacirer::GameObject& selectedGameObject)
    {
        if (!ImGui::BeginMenu("Rendering"))
        {
            return;
        }

        if(ImGui::MenuItem("Mesh"))
        {
            selectedGameObject.AddComponent<Glacirer::MeshComponent>();
        }

        if(ImGui::MenuItem("Camera"))
        {
            selectedGameObject.AddComponent<Glacirer::CameraComponent>();
        }

        if(ImGui::MenuItem("Directional Light"))
        {
            selectedGameObject.AddComponent<Glacirer::DirectionalLightComponent>();
        }

        if (ImGui::MenuItem("Point Light"))
        {
            selectedGameObject.AddComponent<Glacirer::PointLightComponent>();
        }

        if(ImGui::MenuItem("Spot Light"))
        {
            selectedGameObject.AddComponent<Glacirer::SpotLightComponent>();
        }

        if(ImGui::MenuItem("Skybox"))
        {
            selectedGameObject.AddComponent<Glacirer::SkyboxComponent>();
        }

        if(ImGui::MenuItem("Post Processing"))
        {
            selectedGameObject.AddComponent<Glacirer::PostProcessingComponent>();
        }

        ImGui::EndMenu();
    }
}
