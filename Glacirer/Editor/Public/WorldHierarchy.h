#pragma once

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    class WorldHierarchy
    {
    public:
        void RenderGUI(Glacirer::World& world);

        bool HasAnyGameObjectSelected() const { return m_SelectedIndex >= 0; }
        int GetCurrentSelectedGameObjectIndex() const { return m_SelectedIndex; }
        void ResetSelection();

    private:
        int m_SelectedIndex{-1};

        void TryDeselectingNode();
    };
}
