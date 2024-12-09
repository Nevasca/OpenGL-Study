#pragma once

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
    };
}
