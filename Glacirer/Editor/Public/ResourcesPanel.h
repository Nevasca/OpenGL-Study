#pragma once

#include "ResourceCollection.h"
#include "inspectors/resources/MaterialInspector.h"

namespace GlacirerEditor
{
    class ResourcesPanel
    {
    public:

        void RenderGUI();

    private:

        ResourceCollection m_ResourceCollection{};
        Inspector::MaterialInspector m_MaterialInspector{};
        float m_ResourcesPanelWidth{350.f};

        void RenderMaterialsTabItem();
        void RenderResourceInspectorTabItem();
    };
}
