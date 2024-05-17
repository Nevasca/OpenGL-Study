#pragma once
#include <string>

namespace Rendering
{
    struct CubemapLoadSettings
    {
        std::string RightTextureFilePath{};
        std::string LeftTextureFilePath{};
        std::string TopTextureFilePath{};
        std::string BottomTextureFilePath{};
        std::string BackTextureFilePath{};
        std::string FrontTextureFilePath{};
    };

    struct CubemapTextureData
    {
        unsigned char* RightTextureData{nullptr};
        unsigned char* LeftTextureData{nullptr};
        unsigned char* TopTextureData{nullptr};
        unsigned char* BottomTextureData{nullptr};
        unsigned char* BackTextureData{nullptr};
        unsigned char* FrontTextureData{nullptr};
        unsigned int Width{0};
        unsigned int Height{0};
    };
    
    class Cubemap
    {
    public:

        Cubemap(const CubemapTextureData& data);
        ~Cubemap();

        void Bind(unsigned int slot = 0) const;
        void Unbind(unsigned int slot = 0) const;

        unsigned int GetRendererId() const { return m_RendererId; }
        void SetName(const std::string& name) { m_Name = name; }
        std::string GetName() const { return m_Name; }

    private:

        void CreateSideTexture(unsigned int sideTarget, unsigned char* data, unsigned int width, unsigned int height);
        
        unsigned int m_RendererId{0};
        std::string m_Name{};
    };
}
