#include "Texture.h"

#include <cassert>

Texture::Texture(unsigned char* data, unsigned width, unsigned height, const TextureSettings& settings)
    : m_Width(width), m_Height(height)
{
    Create(data, settings);
}

void Texture::Create(unsigned char* data, const TextureSettings& settings)
{
    GLCall(glGenTextures(1, &m_RendererID));

    m_Target = settings.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    
    GLCall(glBindTexture(m_Target, m_RendererID));

    if(settings.Samples > 1)
    {
        GLCall(glTexImage2DMultisample(m_Target, settings.Samples, settings.InternalFormat, m_Width, m_Height, GL_TRUE));
    }
    else
    {
        GLCall(glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, settings.MinFilter));
        GLCall(glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, settings.MagFilter));
        GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, settings.WrapS));
        GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, settings.WrapT));
        GLCall(glTexImage2D(m_Target, 0, settings.InternalFormat, m_Width, m_Height, 0, settings.Format, settings.Type, data));
    }

    if(settings.GenerateMipmap)
    {
        assert(settings.Samples == 1);
        GLCall(glGenerateMipmap(m_Target));
    }

    GLCall(glBindTexture(m_Target, 0));
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // Set slot X as active texture slot
    GLCall(glBindTexture(m_Target, m_RendererID));

    // On OpenGL 4.5 onwards, we can do this single call instead
    //glBindTextureUnit(Slot, m_RendererID);
}

void Texture::Unbind(unsigned int slot) const
{
    // to properly unbind a texture, we also need to set it active and then bind that active slot to 0
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));  
    GLCall(glBindTexture(m_Target, 0));
}