#pragma once
#include <string>

namespace Glacirer
{
    namespace Rendering
    {
        struct TextureSettings;

        struct CubemapLoadSettings
        {
            std::string RightTextureFilePath{};
            std::string LeftTextureFilePath{};
            std::string TopTextureFilePath{};
            std::string BottomTextureFilePath{};
            std::string FrontTextureFilePath{};
            std::string BackTextureFilePath{};
            bool bIsSRGB{true};
        };

        class Cubemap
        {
        public:

            static constexpr unsigned int TOTAL_SIDES = 6;
            
            Cubemap(unsigned int width, unsigned int height, const TextureSettings& settings, const bool bCreateSideTextures = true);
            ~Cubemap();

            void Bind(unsigned int slot = 0) const;
            void Unbind(unsigned int slot = 0) const;

            void CreateSideTexture(
                const unsigned int sideIndex,
                const unsigned char* data,
                const unsigned int width,
                const unsigned int height,
                const TextureSettings& settings);

            unsigned int GetRendererId() const { return m_RendererId; }
            void SetName(const std::string& name) { m_Name = name; }
            std::string GetName() const { return m_Name; }

        private:

            unsigned int m_RendererId{0};
            std::string m_Name{};
        };
    }
}
