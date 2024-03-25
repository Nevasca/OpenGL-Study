#pragma once

class LegacyIndexBuffer
{
public:
    // Size = bytes
    // Count = elements count
    LegacyIndexBuffer(const unsigned int* Data, unsigned int Count);
    ~LegacyIndexBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return m_Count; }

private:

    // Stores the ID that represents this object on OpenGL side (can be a vertex buffer, vertex array, a texture, shader...)).
    // This ID approach is also used on other graphic APIs, such DirectX, that's why a generic name is used here
    unsigned int m_RendererID{0};
    unsigned int m_Count; // How many indexes it has
};
