#include "Texture.h"

#include "Core.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& FilePath, bool bUseAlpha, bool bFlipVertically)
    : m_FilePath(FilePath)
{
    // Makes the image upside down
    // We need to do this because the 0,0 on OpenGL is the bottom left, instead of the top left on .png format
    // In 2D games, we can make ortho projection to start on top left, so we don't need to flip if that's the case
    stbi_set_flip_vertically_on_load(bFlipVertically);

    int DesiredChannels = bUseAlpha ? 4 : 3;

    // We pass pointers for the width, height and so on so the function can set the proper values on them after figuring it out when loading the texture
    // Desired channels is how many channels we expect this image to have, 4 as we expect RGBA
    m_LocalBuffer = stbi_load(FilePath.c_str(), &m_Width, &m_Height, &m_Channels, DesiredChannels);

    unsigned int InternalFormat = bUseAlpha ? GL_RGBA8 : GL_RGB;
    unsigned int Format = bUseAlpha ? GL_RGBA : GL_RGB;
    
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // To set params for our generated texture, we call the glTexParameter with the suffix of the type of data we are trying to set
    // 'i' is for integer
    // target
    // param name (pname): GL_TEXTURE_MIN_FILTER is the minification filter, how our texture is going to be resample down if it needs to be rendered smaller
    //                     GL_MAG_FILTER is for it needs to be rendered larger
    // param: set to linear resample it
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // Horizontal wrap set to clamp
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // Vertical wrap set to clamp

    // We must specify the four params above, otherwise we will get a black texture (or other unwanted behavior)

    // Send the pixel data we have on local buffer to OpenGL
    // target
    // level: 0 since its not a multilayer texture (read doc for further explanation)
    // internal format: what format OpenGL should store the data. GL_RGBA is deprecated, specify how many bits per pixel using GL_RGBA8
    // width
    // height
    // border: how many pixel of border, we don't want it, so 0
    // format: the format of the data we are providing, m_LocalBuffer is GL_RGBA
    // type: type of the data, each channel is unsigned byte
    // pixels: the pixels data. If we didn't have it right now, we could pass a nullptr so we just allocated memory on GPU for now so we can later provide it
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, m_Width, m_Height, 0, Format, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // Generate mipmaps for the bound texture

    // We are done setting up the texture, so unbind
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // Release local buffer from CPU memory
    // We don't need it anymore, but maybe in some cases we do want to keep it. A flag for that would be useful
    if(m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int Slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + Slot)); // Set slot X as active texture slot
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // On OpenGL 4.5 onwards, we can do this single call instead
    //glBindTextureUnit(Slot, m_RendererID);
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetType(const std::string& Type)
{
    m_Type = Type;
}

void Texture::SetPath(const std::string& Path)
{
    m_Path = Path;
}
