#pragma once
#include "GameObjectInspector.h"
#include "WorldHierarchy.h"
#include "inspectors/WorldInspector.h"

class World;

namespace Editor
{
    class MainPanel
    {
    public:

        void RenderGUI(World& world);
        bool HasAnyGameObjectSelected() const { return m_Hierarchy.HasAnyGameObjectSelected(); }
        int GetCurrentSelectedGameObjectIndex() const { return m_Hierarchy.GetCurrentSelectedGameObjectIndex(); } 

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
        Inspector::WorldInspector m_WorldInspector{};
        float m_MainPanelWidth{350.f};

        void RenderHierarchyTabItem(World& world);
        void RenderInspectorTabItem(World& world);
        void RenderWorldSettingsTabItem(World& world);
    };
}
