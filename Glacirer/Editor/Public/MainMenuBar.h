#pragma once
#include "MenuBar/GameObjectMenuBar.h"

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    class MainMenuBar
    {
    public:

        void RenderGUI(Glacirer::World& world);

    private:

        void RenderViewportMenu(Glacirer::World& world);

        MenuBar::GameObjectMenuBar m_GameObjectMenuBar{};
    };
}
