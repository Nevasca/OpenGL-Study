#include "ResourceCollection.h"

#include "Resources/ResourceManager.h"
#include "Rendering/Material.h"
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    void ResourceCollection::RenderMaterialsGUI()
    {
        RenderMaterialsTools();
        ImGui::Separator();
        
        ImGui::BeginChild("MaterialsList");
        RenderMaterialsList();
        ImGui::EndChild();
    }

    void ResourceCollection::RenderMaterialsTools()
    {
        if(ImGui::Button("+ Add"))
        {
            unsigned int nextMaterialId = Glacirer::Resources::ResourceManager::GetNextMaterialId();
            Glacirer::Resources::ResourceManager::CreateMaterial("Material" + std::to_string(nextMaterialId));
        }
    }

    void ResourceCollection::RenderMaterialsList()
    {
        const std::unordered_map<std::string, std::shared_ptr<Glacirer::Rendering::Material>>& materials = Glacirer::Resources::ResourceManager::GetAllMaterials();

        for(const auto& materialPair : materials)
        {
            const std::shared_ptr<Glacirer::Rendering::Material>& material = materialPair.second;

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
}
