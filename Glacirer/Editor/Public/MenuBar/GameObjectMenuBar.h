#pragma once

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
            void RenderCameraSubmenu(Glacirer::World& world);
            void RenderSkyboxSubmenu(Glacirer::World& world);
            void RenderPostProcessingSubmenu(Glacirer::World& world);
        };
    }
}
