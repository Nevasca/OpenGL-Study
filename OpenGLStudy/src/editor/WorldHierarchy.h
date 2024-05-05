#pragma once

class World;

namespace Editor
{
    class WorldHierarchy
    {
    public:
        void RenderGUI(World& world);

        bool HasAnyGameObjectSelected() const { return m_SelectedIndex >= 0; }
        int GetCurrentSelectedGameObjectIndex() const { return m_SelectedIndex; } 

    private:
        int m_SelectedIndex{-1};

        void TryDeselectingNode();
    };
}
