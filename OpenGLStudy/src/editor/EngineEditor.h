#pragma once
#include "MainPanel.h"
#include "ResourcesPanel.h"

class World;

namespace Editor
{
    class EngineEditor
    {
    public:

        void Initialize();
        void RenderGUI(World& world);

    private:

        MainPanel m_MainPanel{};
        ResourcesPanel m_ResourcesPanel{};
    };
}
