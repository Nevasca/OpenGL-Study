#include "Texture.h"

Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, bool bUseAlpha)
    : m_Width(width), m_Height(height)
{
    TextureSettings settings{};
    settings.InternalFormat = bUseAlpha ? GL_RGBA8 : GL_RGB;
    settings.Format = bUseAlpha ? GL_RGBA : GL_RGB;

    Create(data, settings);
}

Texture::Texture(unsigned char* data, unsigned width, unsigned height, const TextureSettings& settings)
    : m_Width(width), m_Height(height)
{
    Create(data, settings);
}

void Texture::Create(unsigned char* data, const TextureSettings& settings)
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.MinFilter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.MagFilter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.WrapS));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.WrapT));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, settings.InternalFormat, m_Width, m_Height, 0, settings.Format, settings.Type, data));

    if(settings.GenerateMipmap)
    {
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    }

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // Set slot X as active texture slot
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // On OpenGL 4.5 onwards, we can do this single call instead
    //glBindTextureUnit(Slot, m_RendererID);
}

void Texture::Unbind(unsigned int slot) const
{
    // to proper unbind a texture, we also need to set it active and then bind that active slot to 0
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));  
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}