#include "ResourceCollection.h"

#include "Resources/ResourceManager.h"
#include "Rendering/Material.h"
#include <imgui/imgui.h>

namespace GlacirerEditor
{
    ResourceCollection::ResourceCollection()
    {
        m_EngineHiddenMaterials.insert(Glacirer::Resources::ResourceManager::MISSING_MATERIAL_NAME);
        m_EngineHiddenMaterials.insert("M_PostProcessing");
        m_EngineHiddenMaterials.insert("M_Skybox");
    }

    void ResourceCollection::RenderMaterialsGUI()
    {
        RenderMaterialsTools();
        ImGui::Separator();
        
        ImGui::BeginChild("MaterialsList");
        RenderMaterialsList();
        ImGui::EndChild();
    }

    void ResourceCollection::ResetMaterialSelection()
    {
        m_SelectedMaterialId = -1;
        m_SelectedMaterialName = "";
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
            std::string materialName = material->GetName();

            if(m_EngineHiddenMaterials.find(materialName) != m_EngineHiddenMaterials.end())
            {
                continue;
            }

            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

            if(m_SelectedMaterialId == static_cast<int>(material->GetId()))
            {
                nodeFlags |= ImGuiTreeNodeFlags_Selected;
            }

            ImGui::TreeNodeEx((void*)(uintptr_t)material->GetId(), nodeFlags, "%s", materialName.c_str());

            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                m_SelectedMaterialId = static_cast<int>(material->GetId());
                m_SelectedMaterialName = materialName;
            }
        }
    }
}
