#pragma once
#include <memory>
#include <string>

class Texture;

class TextureResource
{
public:

    static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath, bool bUseAlpha, bool bFlipVertically = false);
};
