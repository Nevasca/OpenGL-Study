#pragma once

class VertexBuffer;
class VertexBufferLayout;

class LegacyVertexArray
{
public:
    LegacyVertexArray();
    ~LegacyVertexArray();

    void AddBuffer(const VertexBuffer& Buffer, const VertexBufferLayout& Layout);
    void Bind() const;
    void Unbind() const;

private:

    // Stores the ID that represents this object on OpenGL side (can be a vertex buffer, vertex array, a texture, shader...)).
    // This ID approach is also used on other graphic APIs, such DirectX, that's why a generic name is used here
    unsigned int m_RendererID;
};
