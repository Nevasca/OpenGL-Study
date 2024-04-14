#include "TextureResource.h"
#include <stb_image/stb_image.h>

#include "core/Rendering/Texture.h"

std::shared_ptr<Texture> TextureResource::LoadTextureFromFile(const std::string& filePath, bool bUseAlpha, bool bFlipVertically)
{
    stbi_set_flip_vertically_on_load(bFlipVertically);

    int desiredChannels = bUseAlpha ? 4 : 3;

    int width;
    int height;
    int channels;

    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, desiredChannels);

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(data, width, height, bUseAlpha);
    texture->SetIsFlippedOnLoad(bFlipVertically);
    
    if(data)
    {
        stbi_image_free(data);
    }

    return texture;
}
