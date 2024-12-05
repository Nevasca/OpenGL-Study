#include "ResourcesPanel.h"

#include "core/ResourceManager.h"
#include <memory>
#include <imgui/imgui.h>

namespace Editor
{
    void ResourcesPanel::RenderGUI()
    {
        const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_ResourcesPanelWidth, mainViewport->WorkSize.y), ImGuiCond_Always);

        if(ImGui::Begin("ResourcesPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
        {
            m_ResourcesPanelWidth = ImGui::GetWindowWidth();
            
            ImGui::BeginChild("Resources", ImVec2(0.f, 350.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

            if(ImGui::BeginTabBar("Materials"))
            {
                RenderMaterialsTabItem();
                ImGui::EndTabBar();
            }

            ImGui::EndChild();

            ImGui::BeginChild("ResourceDetails", ImVec2(0, 0), ImGuiChildFlags_Border);

            if(ImGui::BeginTabBar("ResourceDetailsTabs"))
            {
                RenderResourceInspectorTabItem();
                ImGui::EndTabBar();
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void ResourcesPanel::RenderMaterialsTabItem()
    {
        if(ImGui::BeginTabItem("Materials"))
        {
            m_ResourceCollection.RenderMaterialsGUI();
            ImGui::EndTabItem();
        }
    }

    void ResourcesPanel::RenderResourceInspectorTabItem()
    {
        if(ImGui::BeginTabItem("Details"))
        {
            if(m_ResourceCollection.HasAnyMaterialSelected())
            {
                std::shared_ptr<Material> selectedMaterial = ResourceManager::GetMaterial(m_ResourceCollection.GetCurrentSelectedMaterialName());
                assert(selectedMaterial);
                m_MaterialInspector.RenderGUI(*selectedMaterial);
            }
            
            ImGui::EndTabItem();
        }
    }
}
