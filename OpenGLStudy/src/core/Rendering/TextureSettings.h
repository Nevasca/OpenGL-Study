#pragma once

#include "OpenGLCore.h"

struct TextureSettings
{
    unsigned int InternalFormat{GL_SRGB};
    unsigned int Format{GL_RGB};
    unsigned int Type{GL_UNSIGNED_BYTE};
    unsigned int MinFilter{GL_LINEAR};
    unsigned int MagFilter{GL_LINEAR};
    unsigned int WrapS{GL_CLAMP_TO_EDGE};
    unsigned int WrapT{GL_CLAMP_TO_EDGE};
    bool GenerateMipmap{false};
    unsigned int Samples{1};
    bool UseAlpha{false};

    TextureSettings() = default;
    TextureSettings(bool bUseAlpha, bool bIsSRGB = true);
};
