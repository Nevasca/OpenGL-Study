#pragma once
#include "GameObjectInspector.h"
#include "WorldHierarchy.h"
#include "inspectors/WorldInspector.h"

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    class MainPanel
    {
    public:

        void RenderGUI(Glacirer::World& world);
        bool HasAnyGameObjectSelected() const { return m_Hierarchy.HasAnyGameObjectSelected(); }
        int GetCurrentSelectedGameObjectIndex() const { return m_Hierarchy.GetCurrentSelectedGameObjectIndex(); } 

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
        Inspector::WorldInspector m_WorldInspector{};
        float m_MainPanelWidth{350.f};

        void RenderHierarchyTabItem(Glacirer::World& world);
        void RenderInspectorTabItem(Glacirer::World& world);
        void RenderWorldSettingsTabItem(Glacirer::World& world);
    };
}
