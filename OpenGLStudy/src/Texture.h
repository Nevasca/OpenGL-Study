#pragma once

#include <string>

#include "Renderer.h"

class Texture
{
public:

    Texture(const std::string& FilePath, bool bFlipVertically = true, bool bUseAlpha = true);
    ~Texture();

    // In OpenGL we have slots to bind texture, we can bind more than one texture at once
    // On Windows, on modern GPUs we generally have 32 slots
    // On mobile, we may have only 8
    // But it all depends on the current GPU. We have a function that we can call to check how many slots we have for the current GPU
    void Bind(unsigned int Slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:

    unsigned int m_RendererID{0};
    std::string m_FilePath{};
    unsigned char* m_LocalBuffer{nullptr};
    int m_Width{0};
    int m_Height{0};
    int m_BPP{0}; // Bytes per pixel
};
