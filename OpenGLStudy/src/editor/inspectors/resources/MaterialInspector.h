#pragma once

class Material;

namespace Editor
{
    namespace Inspector
    {
        class MaterialInspector
        {
        public:
            void RenderGUI(Material& material);

        private:
            void RenderRenderingMode(Material& material);
            void RenderColorProperties(Material& material);
            void RenderTextureProperties(Material& material);
            void RenderBoolProperties(Material& material);
            void RenderFloatProperties(Material& material);
            void RenderIntProperties(Material& material);
        };
    }
}
