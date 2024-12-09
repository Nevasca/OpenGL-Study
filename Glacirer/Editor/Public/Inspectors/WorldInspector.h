#pragma once

namespace Glacirer
{
    class World;
}

namespace GlacirerEditor
{
    namespace Inspector
    {
        class WorldInspector
        {
        public:

            void RenderGUI(Glacirer::World& world);

        private:

            void RenderLightingProperties(Glacirer::World& world);
            void RenderRenderingProperties(Glacirer::World& world);
        };
    }
}
