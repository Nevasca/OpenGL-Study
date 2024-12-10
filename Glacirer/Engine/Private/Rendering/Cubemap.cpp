#include "Rendering/Cubemap.h"

#include "Rendering/OpenGLCore.h"
#include "Rendering/TextureSettings.h"

namespace Glacirer
{
    namespace Rendering
    {
        Cubemap::Cubemap(unsigned int width, unsigned int height, const TextureSettings& settings, const bool bCreateSideTextures)
        {
            GLCall(glGenTextures(1, &m_RendererId));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));

            if(bCreateSideTextures)
            {
                for (unsigned int i = 0; i < 6; i++)
                {
                    CreateSideTexture(i, nullptr, width, height, settings);
                }
            }

            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, settings.MinFilter));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, settings.MagFilter));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, settings.WrapS));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, settings.WrapT));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, settings.WrapR));

            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        }

        Cubemap::~Cubemap()
        {
            GLCall(glDeleteTextures(1, &m_RendererId));
        }

        void Cubemap::Bind(unsigned int slot) const
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
        }

        void Cubemap::Unbind(unsigned int slot) const
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        }

        void Cubemap::CreateSideTexture(
            const unsigned int sideIndex,
            const unsigned char* data,
            const unsigned int width,
            const unsigned int height,
            const TextureSettings& settings)
        {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + sideIndex,0, settings.InternalFormat, static_cast<int>(width), static_cast<int>(height), 0, settings.Format, settings.Type, data));
        }
    }
}
