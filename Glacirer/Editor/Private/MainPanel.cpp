#include "MainPanel.h"

#include <imgui/imgui.h>
#include "World.h"

namespace GlacirerEditor
{
    void MainPanel::RenderGUI(Glacirer::World& world)
    {
        const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkSize.x - m_MainPanelWidth, mainViewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_MainPanelWidth, mainViewport->WorkSize.y), ImGuiCond_Always);

        if(ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
        {
            m_MainPanelWidth = ImGui::GetWindowWidth();
            
            ImGui::BeginChild("Overview", ImVec2(0.f, 350.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

            if(ImGui::BeginTabBar("OverviewTabs"))
            {
                RenderHierarchyTabItem(world);

                ImGui::EndTabBar();
            }

            ImGui::EndChild();

            ImGui::BeginChild("Details", ImVec2(0, 0), ImGuiChildFlags_Border);

            if(ImGui::BeginTabBar("DetailsTabs"))
            {
                RenderInspectorTabItem(world);
                RenderWorldSettingsTabItem(world);

                ImGui::EndTabBar();
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void MainPanel::RenderHierarchyTabItem(Glacirer::World& world)
    {
        if(ImGui::BeginTabItem("Hierarchy"))
        {
            m_Hierarchy.RenderGUI(world);
            ImGui::EndTabItem();
        }
    }

    void MainPanel::RenderInspectorTabItem(Glacirer::World& world)
    {
        if(!ImGui::BeginTabItem("Inspector"))
        {
            return;
        }

        if(!m_Hierarchy.HasAnyGameObjectSelected())
        {
            ImGui::EndTabItem();
            return;
        }

        int selectedIndex = m_Hierarchy.GetCurrentSelectedGameObjectIndex();
        std::shared_ptr<Glacirer::GameObject> selectedObject = world.GetGameObjectAt(selectedIndex);
        assert(selectedObject);

        m_GameObjectInspector.RenderGUI(*selectedObject);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(ImGui::Button("Add Component", ImVec2(m_MainPanelWidth * 0.5f, 25.f)))
        {
            m_ComponentAdderPopup.OpenPopup();
        }

        m_ComponentAdderPopup.RenderGUI(*selectedObject);

        ImGui::EndTabItem();
    }

    void MainPanel::RenderWorldSettingsTabItem(Glacirer::World& world)
    {
        if(ImGui::BeginTabItem("World Settings"))
        {
            m_WorldInspector.RenderGUI(world);

            ImGui::EndTabItem();
        }
    }
}
