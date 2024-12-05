#pragma once

class World;

namespace Editor
{
    class MainMenuBar
    {
    public:

        void RenderGUI(World& world);

    private:

        void RenderViewportMenu(World& world);
    };
}
