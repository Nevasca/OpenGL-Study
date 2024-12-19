#pragma once

#include "EngineAPI.h"
#include "OpenGLCore.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Glacirer
{
    namespace Rendering
    {
        struct ENGINE_API TextureSettings
        {
            unsigned int InternalFormat{GL_SRGB};
            unsigned int Format{GL_RGB};
            unsigned int Type{GL_UNSIGNED_BYTE};
            unsigned int MinFilter{GL_LINEAR};
            unsigned int MagFilter{GL_LINEAR};
            unsigned int WrapS{GL_CLAMP_TO_EDGE};
            unsigned int WrapT{GL_CLAMP_TO_EDGE};
            unsigned int WrapR{GL_CLAMP_TO_EDGE};
            bool UseBorder{false};
            glm::vec4 BorderColor{0.f};
            bool GenerateMipmap{false};
            unsigned int Samples{1};
            bool UseAlpha{false};
            bool bIsSRGB{true};
            bool bAutoDesiredChannels{true};

            TextureSettings() = default;
            TextureSettings(bool bUseAlpha, bool bInIsSRGB = true);

            void UpdateFormats();
        };
    }
}
