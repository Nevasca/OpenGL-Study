#pragma once
#include "Rendering/Material.h"

namespace Glacirer
{
    namespace Rendering
    {
        class Material;
    }
}

namespace GlacirerEditor
{
    namespace Inspector
    {
        class MaterialInspector
        {
        public:
            void RenderGUI(Glacirer::Rendering::Material& material);

        private:
            void RenderRenderingMode(Glacirer::Rendering::Material& material);
            void RenderColorProperties(Glacirer::Rendering::Material& material);
            void TryApplyingTextureFor(Glacirer::Rendering::Material& material, const std::string& name, const unsigned int slot);
            void RenderTextureProperties(Glacirer::Rendering::Material& material);
            void RenderBoolProperties(Glacirer::Rendering::Material& material);
            void RenderFloatProperties(Glacirer::Rendering::Material& material);
            void RenderIntProperties(Glacirer::Rendering::Material& material);
        };
    }
}
