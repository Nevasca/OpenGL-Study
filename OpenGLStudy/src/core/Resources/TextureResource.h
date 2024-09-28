#pragma once
#include <memory>
#include <string>

#include "core/Rendering/Texture.h"

namespace Rendering
{
    struct CubemapLoadSettings;
    class Cubemap;
}

class Texture;

class TextureResource
{
public:

    static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath, const TextureSettings& settings, bool bFlipVertically = false);
    static std::shared_ptr<Rendering::Cubemap> LoadCubemapFromFile(const Rendering::CubemapLoadSettings& loadSettings);
};
