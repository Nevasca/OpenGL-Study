#pragma once
#include <string>

namespace GlacirerEditor
{
    class ResourceCollection
    {
    public:
        void RenderMaterialsGUI();

        bool HasAnyMaterialSelected() const { return m_SelectedMaterialId >= 0; }
        std::string GetCurrentSelectedMaterialName() const { return m_SelectedMaterialName; }
        void ResetMaterialSelection();

    private:
        int m_SelectedMaterialId{-1};
        std::string m_SelectedMaterialName{};

        void RenderMaterialsTools();
        void RenderMaterialsList();
    };
}
