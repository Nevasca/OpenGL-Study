#pragma once
#include <memory>
#include <string>

#include "Rendering/Texture.h"

namespace Glacirer
{
    namespace Rendering
    {
        struct CubemapLoadSettings;
        class Cubemap;
        class Texture;
    }

    namespace Resources
    {
        class TextureResource
        {
        public:

            static std::shared_ptr<Rendering::Texture> LoadTextureFromFile(const std::string& filePath, const Rendering::TextureSettings& settings, bool bFlipVertically = false);
            static std::shared_ptr<Rendering::Cubemap> LoadCubemapFromFile(const Rendering::CubemapLoadSettings& loadSettings);
        };
    }
}
