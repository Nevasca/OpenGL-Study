#pragma once
#include <memory>
#include <string>

namespace Rendering
{
    struct CubemapLoadSettings;
    class Cubemap;
}

class Texture;

class TextureResource
{
public:

    static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath, bool bUseAlpha, bool bFlipVertically = false);
    static std::shared_ptr<Rendering::Cubemap> LoadCubemapFromFile(const Rendering::CubemapLoadSettings& loadSettings);
};
