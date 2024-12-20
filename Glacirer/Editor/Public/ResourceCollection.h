#pragma once
#include <string>
#include <unordered_set>

namespace GlacirerEditor
{
    class ResourceCollection
    {
    public:
        ResourceCollection();

        void RenderMaterialsGUI();
        void ResetMaterialSelection();

        bool HasAnyMaterialSelected() const { return m_SelectedMaterialId >= 0; }
        std::string GetCurrentSelectedMaterialName() const { return m_SelectedMaterialName; }

    private:
        int m_SelectedMaterialId{-1};
        std::string m_SelectedMaterialName{};
        std::unordered_set<std::string> m_EngineHiddenMaterials{};

        void RenderMaterialsTools();
        void RenderMaterialsList();
    };
}
