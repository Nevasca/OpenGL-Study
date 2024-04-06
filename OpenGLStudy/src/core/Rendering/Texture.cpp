#include "Texture.h"

#include "Core.h"

Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, bool bUseAlpha)
    : m_Width(width), m_Height(height)
{
    unsigned int internalFormat = bUseAlpha ? GL_RGBA8 : GL_RGB;
    unsigned int format = bUseAlpha ? GL_RGBA : GL_RGB;
    
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // TODO: these params and to generate or not mipmap could be a struct, like TextureSettings
    // so we can have a more flexible texture generation
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data));
    // GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // Generate mipmaps for the bound texture

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