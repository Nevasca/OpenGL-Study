#pragma once
#include "GameObjectInspector.h"
#include "WorldHierarchy.h"

class World;

namespace Editor
{
    class MainPanel
    {
    public:

        void RenderGUI(World& world);

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
        float m_MainPanelWidth{350.f};

        void RenderHierarchyTabItem(World& world);
        void RenderInspectorTabItem(World& world);
    };
}
