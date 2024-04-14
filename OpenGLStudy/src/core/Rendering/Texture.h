#pragma once
#include <string>

class Texture
{
public:

    Texture(unsigned char* data, unsigned int width, unsigned int height, bool bUseAlpha);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind(unsigned int slot = 0) const;

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }
    unsigned int GetRendererID() const { return m_RendererID; }
    void SetName(const std::string& name) { m_Name = name; }
    std::string GetName() const { return m_Name; }
    void SetIsFlippedOnLoad(const bool bFlipped) { bIsFlippedOnLoad = bFlipped; }
    bool IsFlippedOnLoad() const { return bIsFlippedOnLoad; }

private:

    unsigned int m_RendererID{0};
    unsigned int m_Width{0};
    unsigned int m_Height{0};
    std::string m_Name{};
    bool bIsFlippedOnLoad{false};
};
