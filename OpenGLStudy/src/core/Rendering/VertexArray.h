#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

    unsigned int GetRendererID() const { return m_RendererID; }
    unsigned int GetNextAttributeLocation() const { return m_NextAttributeLocation; }
    void SetNextAttributeLocation(unsigned int nextAttributeLocation) { m_NextAttributeLocation = nextAttributeLocation; } 

private:

    unsigned int m_RendererID;
    mutable bool bIsBound{false};
    unsigned int m_NextAttributeLocation{0};
};
