#pragma once

#include <string>

class LegacyTexture
{
public:

    LegacyTexture(const std::string& FilePath, bool bUseAlpha = true, bool bFlipVertically = true);
    ~LegacyTexture();

    // In OpenGL we have slots to bind texture, we can bind more than one texture at once
    // On Windows, on modern GPUs we generally have 32 slots
    // On mobile, we may have only 8
    // But it all depends on the current GPU. We have a function that we can call to check how many slots we have for the current GPU
    void Bind(unsigned int Slot = 0) const;
    void Unbind(unsigned int Slot = 0) const;
    void SetType(const std::string& Type);
    void SetPath(const std::string& Path);

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline const std::string& GetType() const { return m_Type; }
    inline const std::string& GetPath() const { return  m_Path; }

private:

    unsigned int m_RendererID{0};
    std::string m_FilePath{};
    unsigned char* m_LocalBuffer{nullptr};
    int m_Width{0};
    int m_Height{0};
    int m_Channels{0};
    std::string m_Type{};
    std::string m_Path{}; // Store path to compare if already loaded when loading models
};
