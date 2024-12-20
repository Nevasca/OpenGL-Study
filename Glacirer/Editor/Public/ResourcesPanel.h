#pragma once

#include "IMaterialProvider.h"
#include "ResourceCollection.h"
#include "inspectors/resources/MaterialInspector.h"

namespace GlacirerEditor
{
    class ResourcesPanel : public IMaterialProvider
    {
    public:
        void RenderGUI();

        bool HasAnyMaterialSelected() const override;
        std::string GetCurrentSelectedMaterialName() const override;

        void ResetMaterialSelection();
    private:

        ResourceCollection m_ResourceCollection{};
        Inspector::MaterialInspector m_MaterialInspector{};
        float m_ResourcesPanelWidth{350.f};

        void RenderMaterialsTabItem();
        void RenderResourceInspectorTabItem();
    };
}
