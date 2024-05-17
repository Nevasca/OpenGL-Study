#include "Cubemap.h"

#include "OpenGLCore.h"

namespace Rendering
{
    Cubemap::Cubemap(const CubemapTextureData& data)
    {
        GLCall(glGenTextures(1, &m_RendererId));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));

        // Since GL_TEXTURE_CUBE_MAP_POSITIVE_X is just a int, we could just loop through the 6 textures incrementing it
        // (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i) to get the same result as calling it separately 
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, data.RightTextureData, data.Width, data.Height);
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, data.LeftTextureData, data.Width, data.Height);
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, data.TopTextureData, data.Width, data.Height);
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, data.BottomTextureData, data.Width, data.Height);

        // TODO: There is something going on with sky cube faces or coordinate system that requires swapping front and back textures
        // Maybe the vertices order?
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, data.FrontTextureData, data.Width, data.Height);
        CreateSideTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, data.BackTextureData, data.Width, data.Height);

        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        
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

    void Cubemap::CreateSideTexture(unsigned int sideTarget, unsigned char* data, unsigned int width, unsigned int height)
    {
        GLCall(glTexImage2D(sideTarget,0, GL_RGB, static_cast<int>(width), static_cast<int>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    }
}
