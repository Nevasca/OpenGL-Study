﻿#pragma once

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    namespace MenuBar
    {
        class GameObjectMenuBar
        {
        public:
            void RenderGUI(Glacirer::World& world);

        private:
            void RenderShapesSubmenu(Glacirer::World& world);
            void RenderLightSubmenu(Glacirer::World& world);
        };
    }
}
