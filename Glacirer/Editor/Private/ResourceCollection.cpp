#include "ResourceCollection.h"

#include "ResourceManager.h"
#include "Rendering/Material.h"
#include <imgui/imgui.h>

void Editor::ResourceCollection::RenderMaterialsGUI()
{
    const std::unordered_map<std::string, std::shared_ptr<Material>>& materials = ResourceManager::GetAllMaterials();

    for(const auto& materialPair : materials)
    {
        const std::shared_ptr<Material>& material = materialPair.second;

        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

        if(m_SelectedMaterialId == static_cast<int>(material->GetId()))
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
            
        ImGui::TreeNodeEx((void*)(uintptr_t)material->GetId(), nodeFlags, "%s", material->GetName().c_str());

        if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            m_SelectedMaterialId = static_cast<int>(material->GetId());
            m_SelectedMaterialName = material->GetName();
        }
    }
}
