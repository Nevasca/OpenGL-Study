#include "Resources/TextureResource.h"

#include <cassert>
#include <stb_image/stb_image.h>

#include "Rendering/Cubemap.h"

namespace Glacirer
{
    namespace Resources
    {
        std::shared_ptr<Rendering::Texture> TextureResource::LoadTextureFromFile(const std::string& filePath, const Rendering::TextureSettings& settings, bool bFlipVertically)
        {
            stbi_set_flip_vertically_on_load(bFlipVertically);

            int desiredChannels = settings.UseAlpha ? 4 : 3;

            int width;
            int height;
            int channels;

            unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, desiredChannels);

            std::shared_ptr<Rendering::Texture> texture = std::make_shared<Rendering::Texture>(data, width, height, settings);
            texture->SetIsFlippedOnLoad(bFlipVertically);
    
            if(data)
            {
                stbi_image_free(data);
            }

            return texture;
        }

        std::shared_ptr<Rendering::Cubemap> TextureResource::LoadCubemapFromFile(const Rendering::CubemapLoadSettings& loadSettings)
        {
            stbi_set_flip_vertically_on_load(false);
    
            int width;
            int height;
            int channels;

            std::string sides[Rendering::Cubemap::TOTAL_SIDES]
            {
                loadSettings.RightTextureFilePath,
                loadSettings.LeftTextureFilePath,
                loadSettings.TopTextureFilePath,
                loadSettings.BottomTextureFilePath,
                loadSettings.FrontTextureFilePath,
                loadSettings.BackTextureFilePath
            };

            Rendering::TextureSettings textureSettings{false, loadSettings.bIsSRGB};

            std::shared_ptr<Rendering::Cubemap> cubemap = std::make_shared<Rendering::Cubemap>(0, 0, textureSettings, false);
            cubemap->Bind();

            for (unsigned int i = 0; i < Rendering::Cubemap::TOTAL_SIDES; i++)
            {
                unsigned char* textureData = stbi_load(sides[i].c_str(), &width, &height, &channels, 0);
                assert(textureData);

                cubemap->CreateSideTexture(i, textureData, width, height, textureSettings);

                stbi_image_free(textureData);
            }

            cubemap->Unbind();

            return cubemap;
        }
    }
}
