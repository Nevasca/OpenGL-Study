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
    
            Rendering::CubemapTextureData cubemapData;

            int width;
            int height;
            int channels;

            // TODO: refactor cubemap load to use vector or a method to set data and create the desired side, so we can load one texture
            // use it and then free memory before loading next. Right now we are loading all 6 textures at same time
            cubemapData.RightTextureData = stbi_load(loadSettings.RightTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.LeftTextureData = stbi_load(loadSettings.LeftTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.TopTextureData = stbi_load(loadSettings.TopTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.BottomTextureData = stbi_load(loadSettings.BottomTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.BackTextureData = stbi_load(loadSettings.BackTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.FrontTextureData = stbi_load(loadSettings.FrontTextureFilePath.c_str(), &width, &height, &channels, 0);
            cubemapData.Width = static_cast<unsigned int>(width);
            cubemapData.Height = static_cast<unsigned int>(height);
            cubemapData.bIsSRGB = loadSettings.bIsSRGB;

            std::shared_ptr<Rendering::Cubemap> cubemap = std::make_shared<Rendering::Cubemap>(cubemapData);

            assert(cubemapData.RightTextureData);
            stbi_image_free(cubemapData.RightTextureData);

            assert(cubemapData.LeftTextureData);
            stbi_image_free(cubemapData.LeftTextureData);

            assert(cubemapData.TopTextureData);
            stbi_image_free(cubemapData.TopTextureData);

            assert(cubemapData.BottomTextureData);
            stbi_image_free(cubemapData.BottomTextureData);

            assert(cubemapData.BackTextureData);
            stbi_image_free(cubemapData.BackTextureData);
    
            assert(cubemapData.FrontTextureData);
            stbi_image_free(cubemapData.FrontTextureData);

            return cubemap;
        }
    }
}
