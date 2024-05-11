#pragma once
#include "MainMenuBar.h"
#include "MainPanel.h"
#include "ResourcesPanel.h"

class World;

namespace Editor
{
    class EngineEditor
    {
    public:

        void Initialize();
        void Update(float deltaTime, World& world);
        void RenderGUI(World& world);

    private:

        MainMenuBar m_MainMenuBar{};
        MainPanel m_MainPanel{};
        ResourcesPanel m_ResourcesPanel{};
        int m_SelectedGameObjectIndex{-1};
        bool bShowPanelsEnabled{true};

        void UpdateSelectedGameObject(const World& world);
        void SelectGameObject(int index, const World& world);
        void DeselectGameObject(int index, const World& world);
        void UpdateShortcuts();
    };
}
