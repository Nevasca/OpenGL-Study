#pragma once

class Texture
{
public:

    Texture(unsigned char* data, unsigned int width, unsigned int height, bool bUseAlpha);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind(unsigned int slot = 0) const;

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }

private:

    unsigned int m_RendererID{0};
    unsigned int m_Width{0};
    unsigned int m_Height{0};
};
