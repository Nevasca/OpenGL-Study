#pragma once

class World;

namespace Editor
{
    namespace Inspector
    {
        class WorldInspector
        {
        public:

            void RenderGUI(World& world);

        private:

            void RenderLightingProperties(World& world);
            void RenderRenderingProperties(World& world);
        };
    }
}
