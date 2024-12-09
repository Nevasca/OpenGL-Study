#pragma once
#include <string>

namespace Editor
{
    class ResourceCollection
    {
    public:
        void RenderMaterialsGUI();

        bool HasAnyMaterialSelected() const { return m_SelectedMaterialId >= 0; }
        std::string GetCurrentSelectedMaterialName() const { return m_SelectedMaterialName; } 

    private:

        int m_SelectedMaterialId{-1};
        std::string m_SelectedMaterialName{};
    };
}
