#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& Buffer, const VertexBufferLayout& Layout);
    void Bind() const;
    void Unbind() const;

    unsigned int GetRendererID() const { return m_RendererID; }

private:

    unsigned int m_RendererID;
    mutable bool bIsBound{false};
};
