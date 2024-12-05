#include "MainPanel.h"

#include <imgui/imgui.h>
#include "World.h"

namespace Editor
{
    void MainPanel::RenderGUI(World& world)
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

    void MainPanel::RenderHierarchyTabItem(World& world)
    {
        if(ImGui::BeginTabItem("Hierarchy"))
        {
            m_Hierarchy.RenderGUI(world);
            ImGui::EndTabItem();
        }
    }

    void MainPanel::RenderInspectorTabItem(World& world)
    {
        if(ImGui::BeginTabItem("Inspector"))
        {
            if(m_Hierarchy.HasAnyGameObjectSelected())
            {
                int selectedIndex = m_Hierarchy.GetCurrentSelectedGameObjectIndex();
                std::shared_ptr<GameObject> selectedObject = world.GetGameObjectAt(selectedIndex);

                m_GameObjectInspector.RenderGUI(*selectedObject);
            }

            ImGui::EndTabItem();
        }
    }

    void MainPanel::RenderWorldSettingsTabItem(World& world)
    {
        if(ImGui::BeginTabItem("World Settings"))
        {
            m_WorldInspector.RenderGUI(world);

            ImGui::EndTabItem();
        }
    }
}
