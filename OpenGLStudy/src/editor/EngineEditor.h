#pragma once
#include "GameObjectInspector.h"
#include "ResourceCollection.h"
#include "WorldHierarchy.h"
#include "inspectors/resources/MaterialInspector.h"

class World;

namespace Editor
{
    class EngineEditor
    {
    public:

        void Initialize();
        void RenderGUI(World& world);

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
        ResourceCollection m_ResourceCollection{};
        Inspector::MaterialInspector m_MaterialInspector{};
        float m_MainPanelWidth{350.f};
        float m_ResourcesPanelWidth{350.f};

        void RenderMainPanel(World& world);
        void RenderHierarchyTabItem(World& world);
        void RenderInspectorTabItem(World& world);
        void RenderResourcesPanel();
        void RenderMaterialsTabItem();
        void RenderResourceInspectorTabItem();
    };
}
