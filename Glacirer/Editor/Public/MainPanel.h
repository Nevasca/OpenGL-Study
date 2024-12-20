#pragma once
#include "ComponentAdderPopup.h"
#include "GameObjectInspector.h"
#include "WorldHierarchy.h"
#include "inspectors/WorldInspector.h"

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    class IMaterialProvider;

    class MainPanel
    {
    public:

        void Setup(const std::weak_ptr<IMaterialProvider>& materialProvider);
        
        void RenderGUI(Glacirer::World& world);
        bool HasAnyGameObjectSelected() const { return m_Hierarchy.HasAnyGameObjectSelected(); }
        int GetCurrentSelectedGameObjectIndex() const { return m_Hierarchy.GetCurrentSelectedGameObjectIndex(); }
        void ResetSelection() { m_Hierarchy.ResetSelection(); }

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
        Inspector::WorldInspector m_WorldInspector{};
        ComponentAdderPopup m_ComponentAdderPopup{};
        float m_MainPanelWidth{350.f};

        void RenderHierarchyTabItem(Glacirer::World& world);
        void RenderInspectorTabItem(Glacirer::World& world);
        void RenderWorldSettingsTabItem(Glacirer::World& world);
    };
}
