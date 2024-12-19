#include "Rendering/TextureSettings.h"

namespace Glacirer
{
    namespace Rendering
    {
        TextureSettings::TextureSettings(bool bUseAlpha, bool bInIsSRGB)
            : UseAlpha(bUseAlpha)
            , bIsSRGB(bInIsSRGB)
            , bAutoDesiredChannels(false)
        {
            UpdateFormats();
        }

        void TextureSettings::UpdateFormats()
        {
            if(UseAlpha)
            {
                InternalFormat = bIsSRGB ? GL_SRGB_ALPHA : GL_RGBA;
                Format = GL_RGBA;
            }
            else
            {
                InternalFormat = bIsSRGB ? GL_SRGB : GL_RGB;
                Format = GL_RGB;
            }
        }
    }
}
