#pragma once
#include "Engine.h"
#include "MainMenuBar.h"
#include "MainPanel.h"
#include "ResourcesPanel.h"
#include "StatisticsWindow.h"

class World;

namespace Editor
{
    class Editor
    {
    public:

        void Initialize();
        void Setup();
        void Update();
        void Render();

        bool IsInitialized() const;
        bool ShouldClose() const;

    private:

        Glacirer::Engine m_Engine{};

        MainMenuBar m_MainMenuBar{};
        MainPanel m_MainPanel{};
        ResourcesPanel m_ResourcesPanel{};
        StatisticsWindow m_StatisticsWindow{};
        int m_SelectedGameObjectIndex{-1};
        bool bShowPanelsEnabled{true};

        void RenderGUI(World& world);
        void UpdateSelectedGameObject(const World& world);
        void SelectGameObject(int index, const World& world);
        void DeselectGameObject(int index, const World& world);
        void UpdateShortcuts();

        void InitializeImGUI();
    };
}
