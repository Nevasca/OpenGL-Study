#include "EngineEditor.h"

#include <memory>

#include "core/ResourceManager.h"
#include "core/World.h"
#include "imgui/imgui.h"

namespace Editor
{
    void EngineEditor::Initialize()
    { }

    void EngineEditor::RenderGUI(World& world)
    {
        RenderMainPanel(world);
        RenderResourcesPanel();
    }

    void EngineEditor::RenderMainPanel(World& world)
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

                ImGui::EndTabBar();
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void EngineEditor::RenderHierarchyTabItem(World& world)
    {
        if(ImGui::BeginTabItem("Hierarchy"))
        {
            m_Hierarchy.RenderGUI(world);
            ImGui::EndTabItem();
        }
    }

    void EngineEditor::RenderInspectorTabItem(World& world)
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

    void EngineEditor::RenderResourcesPanel()
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

    void EngineEditor::RenderMaterialsTabItem()
    {
        if(ImGui::BeginTabItem("Materials"))
        {
            m_ResourceCollection.RenderMaterialsGUI();
            ImGui::EndTabItem();
        }
    }

    void EngineEditor::RenderResourceInspectorTabItem()
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

